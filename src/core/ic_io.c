/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

#define MAX_PRINT_LEN 1024

IC_PUBLIC
IC_PRINTF_FORMAT(2, 3)
void _ic_print(bool warning, const char *fmt, ...)
{
    char msg[MAX_PRINT_LEN];

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    if (warning) {
        #if !defined SYS_WARN
        fprintf(stderr, "WARNING: %s", msg);
        #else
        SYS_WARN(msg);
        #endif
    } else {
        #if !defined SYS_PRINT
        fprintf(stderr, "%s", msg);
        #else
        SYS_PRINT(msg);
        #endif
    }
}

IC_PUBLIC
IC_PRINTF_FORMAT(5, 6)
void _ic_error(bool fatal, const char *filename, const char *function, 
               const int line, const char *fmt, ...)
{
    char msg[MAX_PRINT_LEN];

    /* temp */
    UNUSED_PARAM(filename);
    UNUSED_PARAM(function);
    UNUSED_PARAM(line);

    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    if (fatal) {
        #if !defined COM_ERROR
        fprintf(stderr, "ERROR: %s", msg);
        exit(0);
        #else
        COM_ERROR(ERR_FATAL, msg);
        #endif
    } else {
        #if !defined COM_ERROR
        fprintf(stderr, "ERROR: %s", msg);
        #else
        COM_ERROR(ERR_DROP, msg);
        #endif
    }
}
