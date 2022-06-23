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

IC_PUBLIC
void con_set_title(const char *title)
{
    #ifdef IC_PLATFORM_WINDOWS
    SetConsoleTitle(title);
    #else
    // xterm allows for OSC escape sequences,
    // so if we're a compatible xterm, let's do it
    // otherwise don't bother
    if (console.term == NULL || strstr(console.term, "xterm") == NULL)
        return;

    // just assume if we don't support color we don't also
    // don't have this feature
    if (!console.ansi_color)
        return;

    fprintf(stderr, "\033]2;%s\007", title);
    #endif
}
