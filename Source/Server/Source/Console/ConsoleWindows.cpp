// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>
#include <memory>

#include <Console.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static HANDLE handle_in = nullptr;
static HANDLE handle_out = nullptr;
static DWORD original_mode = 0;
static std::size_t line_length_ = 0;

// ================================
// Console::Initialize
// ================================
bool Console::Initialize() noexcept
{
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

    if ((handle_in = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        return false;

    if ((handle_out = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        return false;

    // utf-8 support
    if (SetConsoleCP(CP_UTF8) == FALSE || SetConsoleOutputCP(CP_UTF8) == FALSE) {
        fputs("Failed to set console code page to UTF-8\n", stderr);
        return false;
    }

    // enable mouse-wheel scrolling
    GetConsoleMode(handle_in, &original_mode);
    SetConsoleMode(handle_in, original_mode & ~ENABLE_MOUSE_INPUT);

    // VT support
    DWORD mode = 0;
    GetConsoleMode(handle_out, &mode);
    mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (SetConsoleMode(handle_out, mode) == FALSE)
        return false;

    FlushConsoleInputBuffer(handle_in);

    buffer_.fill(0);

    return true;
}

// ================================
// Console::Shutdown
// ================================
bool Console::Shutdown() noexcept
{
    SetConsoleMode(handle_in, original_mode);

    CloseHandle(handle_in);
    CloseHandle(handle_out);

    return true;
}

// ================================
// Console::GetInput
// ================================
std::string Console::GetInput() noexcept
{
    DWORD events = 0;
    if (GetNumberOfConsoleInputEvents(handle_in, &events) == FALSE || events == 0)
        return {};

    auto event_buffer = std::make_unique<INPUT_RECORD[]>(events);
    if (events >= MAX_EDIT_LINE) {
        // too much data, just eat it
        ReadConsoleInputA(handle_in, event_buffer.get(), 1, &events);
        return {};
    }

    DWORD count = 0;
    if (ReadConsoleInputA(handle_in, event_buffer.get(), events, &count) == FALSE)
        return {};

    FlushConsoleInputBuffer(handle_in);

    bool new_line = false;
    for (DWORD i = 0; i < count; i++) {
        // skip non-key events and key ups
        if (event_buffer[i].EventType != KEY_EVENT || !event_buffer[i].Event.KeyEvent.bKeyDown)
            continue;

        WORD key = event_buffer[i].Event.KeyEvent.wVirtualKeyCode;

        bool breakout = true;
        switch (key) {
        case VK_RETURN:
            new_line = true;
            cursor_ = 0;
            break;
        case VK_UP:
        case VK_DOWN:
            // TODO: history
            break;
        case VK_LEFT:
            cursor_--;

            if (cursor_ < 0)
                cursor_ = 0;
            break;
        case VK_RIGHT:
            cursor_++;

            if (cursor_ > line_length_)
                cursor_ = line_length_;
            break;
        case VK_HOME:
            cursor_ = 0;
            break;
        case VK_END:
            cursor_ = line_length_;
            break;
        case VK_BACK:
            if (cursor_ > 0) {
                std::size_t newlen = line_length_ > 0 ? line_length_ - 1 : 0;

                if (cursor_ < line_length_) {
                    memmove(buffer_.data() + cursor_ - 1, buffer_.data() + cursor_,
                            line_length_ - cursor_);
                }

                buffer_[newlen] = '\0';
                line_length_ = newlen;
                cursor_--;
            }
            break;
        case VK_DELETE:
            if (cursor_ < line_length_) {
                std::size_t newlen = line_length_ > 0 ? line_length_ - 1 : 0;

                memmove(buffer_.data() + cursor_, buffer_.data() + cursor_ + 1,
                        line_length_ - cursor_);

                line_length_ = newlen;
            }
            break;
        case VK_TAB:
            // TODO: autocomplete
            break;
        default:
            breakout = false;
            break;
        }

        if (breakout)
            break;

        if (line_length_ >= MAX_EDIT_LINE - 1)
            continue;

        char ch = event_buffer[i].Event.KeyEvent.uChar.AsciiChar;
        if (ch == 0)
            continue;

        if (line_length_ > cursor_)
            memmove(buffer_.data() + cursor_ + 1, buffer_.data() + cursor_, line_length_ - cursor_);

        buffer_[cursor_++] = ch;
        line_length_++;
        buffer_[line_length_] = '\0';
    }

    if (!new_line) {
        Show();
        return {};
    }

    if (line_length_ == 0) {
        Show();
        Print("\n");
        return {};
    }

    line_length_ = 0;
    Show();
    // history add

    std::string line = buffer_.data();
    Print(line + "\n");
    return line;
}

// ================================
// Console::Back
// ================================
void Console::Back() {}

// ================================
// Console::Show
// ================================
void Console::Show()
{
    if (!console_on_)
        return;

    CONSOLE_SCREEN_BUFFER_INFO info{};
    GetConsoleScreenBufferInfo(handle_out, &info);

    SMALL_RECT write_area = {0, 0, 0, 0};
    write_area.Left = 0;
    write_area.Top = info.dwCursorPosition.Y;
    write_area.Bottom = info.dwCursorPosition.Y;
    write_area.Right = MAX_EDIT_LINE;

    WORD attribute = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    CHAR_INFO line[MAX_EDIT_LINE] = {0};
    for (std::size_t i = 0; i < MAX_EDIT_LINE; i++) {
        if (i < line_length_) {
            line[i].Char.AsciiChar = buffer_[i];
        } else {
            line[i].Char.AsciiChar = ' ';
        }

        line[i].Attributes = attribute;
    }

    COORD write_size = {MAX_EDIT_LINE, 1};
    COORD write_pos = {0, 0};

    if (line_length_ > info.srWindow.Right) {
        WriteConsoleOutputA(handle_out, line + (line_length_ - info.srWindow.Right), write_size,
                            write_pos, &write_area);
    } else {
        WriteConsoleOutputA(handle_out, line, write_size, write_pos, &write_area);
    }

    COORD cursor = {0, 0};
    cursor.Y = info.dwCursorPosition.Y;
    cursor.X = cursor_ < line_length_               ? cursor_
               : line_length_ > info.srWindow.Right ? info.srWindow.Right
                                                    : static_cast<SHORT>(line_length_);

    SetConsoleCursorPosition(handle_out, cursor);
}

// ================================
// Console::Hide
// ================================
void Console::Hide()
{
    const std::size_t length = line_length_;

    line_length_ = 0;
    Show();
    line_length_ = length;
}
