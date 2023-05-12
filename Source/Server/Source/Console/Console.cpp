// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Console.hpp>

// defined in OS-specific source files:
// Initialize
// Shutdown
// GetInput
// Back
// Show
// Hide

// --------------------------------
// Console::Print
// --------------------------------
void Console::Print(const std::string& text, bool manual_color)
{
    static std::size_t overdue = 0;

    if (text.empty())
        return;

    Hide();

    if (manual_color)
        fputs(text.c_str(), stderr);
    else
        ColorPrint(text);

    if (!console_on_)
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

// --------------------------------
// Console::DebugPrint
// --------------------------------
void Console::DebugPrint([[maybe_unused]] const std::string& text)
{
#ifdef IOCOD_DEBUG
    SetTextColor(VTColor::Cyan);
    Print("DEBUG: " + text);
#endif
}

// --------------------------------
// Console::Warn
// --------------------------------
void Console::Warn(const std::string& text)
{
    SetTextColor(VTColor::BrightYellow);
    Print("WARNING: " + text);
}

// --------------------------------
// Console::Error
// --------------------------------
void Console::Error(const std::string& text)
{
    SetTextColor(VTColor::BrightWhite, VTColor::BrightRed);
    Print("ERROR: " + text);
}

// --------------------------------
// Console::Clear
// --------------------------------
void Console::Clear()
{
    Hide();

    fputs("\033c", stderr);

    Show();
}

// --------------------------------
// Console::SetTextColor
// --------------------------------
void Console::SetTextColor(VTColor foreground, VTColor background)
{
    foreground_ = foreground;
    background_ = background;

    fputs(VTForeColorCode(foreground_), stderr);
    fputs(VTBackColorCode(background_), stderr);
}

// --------------------------------
// Console::SetTitle
// --------------------------------
void Console::SetTitle(const std::string& title)
{
    fputs("\033]0;", stderr);
    fputs(title.c_str(), stderr);
    fputs("\007", stderr);
}

// --------------------------------
// Console::ColorPrint
// --------------------------------
void Console::ColorPrint(const std::string& text)
{
    std::array<char, MAX_EDIT_LINE> buffer{};
    std::size_t buffer_index = 0;

    for (std::size_t i = 0; i < text.size(); i++) {
        bool new_line = text[i] == '\n';
        bool color = IsQ3ColorString(&text[i]);

        console_on_ = new_line;

        if (!color && !new_line) {
            if (buffer_index >= MAX_EDIT_LINE - 1)
                break;

            buffer[buffer_index++] = text[i];
            continue;
        }

        if (color || new_line) {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                fputs(buffer.data(), stderr);
                buffer.fill(0);
                buffer_index = 0;
            }

            if (new_line) {
                SetTextColor(VTColor::White);
                fputs("\n", stderr);
            } else {
                if (text.length() < i + 2)
                    break;

                int color_num = static_cast<int>(text[i + 1] - '0');
                SetTextColor(Q3ColorToVTColor(static_cast<Q3Color>(color_num)));
                i++;
            }
        }
    }

    if (buffer_index) {
        buffer[buffer_index] = '\0';
        fputs(buffer.data(), stderr);
    }
}

