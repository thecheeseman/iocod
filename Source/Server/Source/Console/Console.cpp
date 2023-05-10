// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConsoleLocal.hpp"

#ifdef _WIN32
ConsoleWindows console;
#else
ConsoleLinux console;
#endif

// ================================
// Console::Initialize
// ================================
bool Console::Initialize() noexcept
{
#ifdef _WIN32
    auto console_ctrl_handler = [](DWORD sig) -> BOOL {
        switch (sig) {
        case CTRL_C_EVENT:
            // sys_signal_handler(SIGINT);
            return TRUE;
        case CTRL_CLOSE_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            // sys_signal_handler(SIGTERM);
            return TRUE;
        default:
            return FALSE;
        }
    };

    if (SetConsoleCtrlHandler(console_ctrl_handler, TRUE) == FALSE)
        return false;

    if ((console.hin = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        return false;

    if ((console.hout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        return false;

    GetConsoleMode(console.hin, &console.original_mode);
    SetConsoleMode(console.hin,
                   console.original_mode & ~ENABLE_MOUSE_INPUT); // enable mouse-wheel scrolling

    DWORD mode = 0;
    GetConsoleMode(console.hout, &mode);

    mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (SetConsoleMode(console.hout, mode) == FALSE)
        return false;

    FlushConsoleInputBuffer(console.hin);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(console.hout, &info);
    console.attributes = info.wAttributes;
    console.bg_attributes = console.attributes & (BACKGROUND_BLUE | BACKGROUND_GREEN |
                                                  BACKGROUND_RED | BACKGROUND_INTENSITY);

    SetConsoleTextAttribute(console.hout, console.attributes);
    SetConsoleTextAttribute(console.hout, Q3ColorToAttribute(Q3Color::White));
#else

#endif

    buffer.fill(0);

    return true;
}

// ================================
// Console::Shutdown
// ================================
bool Console::Shutdown() noexcept
{
#ifdef _WIN32
    SetConsoleMode(console.hin, console.original_mode);
    SetConsoleTextAttribute(console.hout, console.attributes);

    CloseHandle(console.hin);
    CloseHandle(console.hout);
#else

#endif
    return true;
}

// ================================
// Console::Print
// ================================
void Console::Print(const std::string& text)
{
    static std::size_t overdue = 0;

    if (text.empty())
        return;

    Hide();

    fputs(text.c_str(), stderr);

    if (!console_on)
        return;

    if (text.back() == '\n') {
        Show();

        while (overdue > 0) {
            Show();
            overdue--;
        }
    } else {
        overdue++;
    }
}

// ================================
// Console::DebugPrint
// ================================
void Console::DebugPrint([[maybe_unused]] const std::string& text)
{
#ifdef IOCOD_DEBUG
    Print(text);
#endif
}

// ================================
// Console::Clear
// ================================
void Console::Clear()
{
    Hide();

#ifdef _WIN32
    COORD coord = {0, 0};
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(console.hout, &info);
    DWORD written = 0;
    FillConsoleOutputCharacter(console.hout, ' ', info.dwSize.X * info.dwSize.Y, coord, &written);
    FillConsoleOutputAttribute(console.hout, console.attributes, info.dwSize.X * info.dwSize.Y,
                               coord,
                               &written);
    SetConsoleCursorPosition(console.hout, coord);
#else
    fputs("\033c", stderr);
#endif

    Show();
}

// ================================
// Console::GetInput
// ================================
std::string Console::GetInput() noexcept
{
    return {};
}

#ifdef _WIN32
static DWORD color_attributes[] = {
    0,
    FOREGROUND_RED,
    FOREGROUND_GREEN,
    FOREGROUND_RED | FOREGROUND_GREEN,
    FOREGROUND_BLUE,
    FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    FOREGROUND_RED | FOREGROUND_BLUE, // console doesn't support these colors
    FOREGROUND_RED                    // so we just reuse the same ones
};

WORD Q3ColorToAttribute(Q3Color color) noexcept
{
    WORD attribute = color_attributes[static_cast<int>(color)];
    attribute |= console.bg_attributes;
    return attribute;
}
#endif

// ================================
// Console::Back
// ================================
void Console::Back()
{
#ifndef _WIN32
    char key = '\b';
    write(STDOUT_FILENO, &key, 1);
    key = ' ';
    write(STDOUT_FILENO, &key, 1);
    key = '\b';
    write(STDOUT_FILENO, &key, 1);
#endif
}

// ================================
// Console::Show
// ================================
void Console::Show()
{
    if (!console_on)
        return;

#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info{};
    GetConsoleScreenBufferInfo(console.hout, &info);

    SMALL_RECT write_area = {0, 0, 0, 0};
    write_area.Left = 0;
    write_area.Top = info.dwCursorPosition.Y;
    write_area.Bottom = info.dwCursorPosition.Y;
    write_area.Right = MAX_EDIT_LINE;

    DWORD attribute = Q3ColorToAttribute(Q3Color::White);

    CHAR_INFO line[MAX_EDIT_LINE] = {0};
    for (std::size_t i = 0; i < MAX_EDIT_LINE; i++) {
        if (i < console.line_length) {
            // TODO: color support
            line[i].Char.AsciiChar = buffer[i];
        } else {
            line[i].Char.AsciiChar = ' ';
        }

        line[i].Attributes = attribute;
    }

    COORD write_size = {MAX_EDIT_LINE, 1};
    COORD write_pos = {0, 0};

    if (console.line_length > info.srWindow.Right) {
        WriteConsoleOutput(console.hout, line + (console.line_length - info.srWindow.Right),
                           write_size,
                           write_pos,
                           &write_area);
    } else {
        WriteConsoleOutput(console.hout, line, write_size, write_pos, &write_area);
    }

    COORD cursor = {0, 0};
    cursor.Y = info.dwCursorPosition.Y;
    cursor.X = console.cursor_pos < console.line_length    ? console.cursor_pos
               : console.line_length > info.srWindow.Right ? info.srWindow.Right
                                                           : console.line_length;

    SetConsoleCursorPosition(console.hout, cursor);
#else
    assert(hide > 0);
    hide--;
    if (hide > 0)
        return;

    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

    if (cursor > 0) {
        for (std::size_t i = 0; i < cursor; i++)
            write(STDOUT_FILENO, &buffer[i], 1);
    }
#endif
}

// ================================
// Console::Hide
// ================================
void Console::Hide() noexcept
{
#ifdef _WIN32
    const std::size_t length = console.line_length;

    console.line_length = 0;
    Show();
    console.line_length = length;
#else
    if (!console_on)
        return;

    if (hide > 0) {
        hide++;
        return;
    }

    if (cursor > 0) {
        for (std::size_t i = 0; i < cursor; i++)
            Back();
    }

    for (std::size_t i = strlen(PROMPT); i > 0; i--)
        Back();

    hide++;
#endif
}
