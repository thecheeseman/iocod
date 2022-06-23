/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

#include "con_local.h"

#ifdef IC_PLATFORM_WINDOWS
void con_back(void)
{

}

void con_hide(void)
{
    int reallen = console.line_len;

    // remove input line from console output buffer
    console.line_len = 0;
    con_show();

    console.line_len = reallen;
}

void con_show(void)
{
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(console.hout, &info);

    if (!console.on)
        return;

    SMALL_RECT write_area = { 0, 0, 0, 0 };
    write_area.Left = 0;
    write_area.Top = info.dwCursorPosition.Y;
    write_area.Bottom = info.dwCursorPosition.Y;
    write_area.Right = MAX_EDIT_LINE;

    DWORD attrib = color_to_attrib(COLOR_WHITE);

    CHAR_INFO line[MAX_EDIT_LINE] = { 0 };
    for (int i = 0; i < MAX_EDIT_LINE; i++) {
        if (i < console.line_len) {
            if ((i + 1) < console.line_len && color_string(console.line + i)) {
                int color = (int) (*(console.line + i + 1) - '0');
                attrib = color_to_attrib(color);
            }

            line[i].Char.AsciiChar = console.line[i];
        } else {
            line[i].Char.AsciiChar = ' ';
        }

        line[i].Attributes = attrib;
    }

    COORD write_size = { MAX_EDIT_LINE, 1 };
    COORD write_pos = { 0, 0 };
    if (console.line_len > info.srWindow.Right) {
        WriteConsoleOutput(console.hout,
                           line + (console.line_len - info.srWindow.Right),
                           write_size, write_pos, &write_area);
    } else {
        WriteConsoleOutput(console.hout, line, write_size, write_pos,
                           &write_area);
    }

    COORD cursor = { 0, 0 };
    cursor.Y = info.dwCursorPosition.Y;
    cursor.X = console.cursor < console.line_len 
        ? console.cursor : console.line_len > info.srWindow.Right 
        ? info.srWindow.Right : console.line_len;

    SetConsoleCursorPosition(console.hout, cursor);
}
#else
void con_back(void)
{
    char key = '\b';
    write(STDOUT_FILENO, &key, 1);
    key = ' ';
    write(STDOUT_FILENO, &key, 1);
    key = '\b';
    write(STDOUT_FILENO, &key, 1);
}

void con_hide(void)
{
    if (!console.on)
        return;

    if (console.hide > 0) {
        console.hide++;
        return;
    }

    if (console.field.cursor > 0) {
        for (size_t i = 0; i < console.field.cursor; i++)
            con_back();
    }

    // delete prompt
    for (int i = strlen(TTY_PROMPT); i > 0; i--)
        con_back();

    console.hide++;
}

void con_show(void)
{
    if (!console.on)
        return;

    assert(console.hide > 0);
    console.hide--;

    if (console.hide == 0) {
        write(STDOUT_FILENO, TTY_PROMPT, strlen(TTY_PROMPT));

        if (console.field.cursor > 0) {
            for (size_t i = 0; i < console.field.cursor; i++)
                write(STDOUT_FILENO, console.field.buffer + i, 1);
        }
    }
}
#endif
