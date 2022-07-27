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

#include "iocod.h"

#include <stdlib.h>

/*
PRINTF_ALL,
PRINTF_CONSOLE,
PRINTF_WARNING,
PRINTF_LOGONLY
*/

IC_PUBLIC
IC_PRINTF_FORMAT(2, 3, fmt)
void _ic_printf(enum printf_type type, const char *fmt, ...)
{
    char msg[MAX_PRINT_MSG];

    va_list argptr = { 0 };
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    if (type == PRINTF_ALL)
        log_print(msg);
    else if (type == PRINTF_WARNING)
        log_warn(msg);
    else if (type == PRINTF_CONSOLE)
        con_print(msg);

    // TODO: log stuff
}
