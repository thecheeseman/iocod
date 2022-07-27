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

static void color_print(const char *msg)
{
    static char buffer[MAX_PRINT_MSG] = { 0 };
    int length = 0;

    while (*msg != '\0') {
        qbool newline = (*msg == '\n');
        qbool color = color_string(msg);
        console.on = newline;

        if (!color && !newline) {
            if (length >= MAX_PRINT_MSG - 1)
                break;

            buffer[length] = *msg;
            length++;
            msg++;
            continue;
        }

        if (color || newline) {
            // empty the buffer
            if (length > 0) {
                buffer[length] = '\0';
                fputs(buffer, stderr);
                length = 0;
            }
            
            if (newline) {
                #ifdef IC_PLATFORM_WINDOWS
                SetConsoleTextAttribute(console.hout,
                                        color_to_attrib(COLOR_WHITE));
                fputs(IC_PLATFORM_NEWLINE, stderr);
                #else
                if (config_console_colors())
                    fputs("\033[0m" IC_PLATFORM_NEWLINE, stderr);
                else
                    fputs(IC_PLATFORM_NEWLINE, stderr);
                #endif
                msg++;
            } else {
                int colorn = (int) (*(msg + 1) - '0');

                #ifdef IC_PLATFORM_WINDOWS
                SetConsoleTextAttribute(console.hout,
                                        color_to_attrib(colorn));
                #else
                if (config_console_colors()) {
                    fputs("\033[", stderr);
                    fputs(color_to_ascii_code(colorn), stderr);
                    fputs("m", stderr);
                }
                #endif
                
                msg += 2;
            }
        }
    }

    if (length > 0) {
        buffer[length] = '\0';
        fputs(buffer, stderr);
    }
}

IC_PUBLIC
IC_NON_NULL(1)
void con_print(_In_z_ const char *msg)
{
    IC_ASSERT(msg != NULL);
    
    // we can potentially call ic_printf before con_init
    // so make sure we always see these messages
    if (!console.initialized) {
        fputs(msg, stderr);
        return;
    }

    con_hide();

    color_print(msg);

    #ifdef IC_PLATFORM_WINDOWS
    con_show();
    #else
    
    if (!console.on)
        return;

    if (msg[strlen(msg) - 1] == '\n') {
        con_show();

        while (console.overdue > 0) {
            con_show();
            console.overdue--;
        }
    } else {
        // defer calling con_show
        console.overdue++;
    }
    #endif
}
