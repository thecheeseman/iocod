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

/* 
    this is smaller than RTCW (32000) but I can't imagine a scenario 
    where you actively need to concatenate more than 8K bytes at once
*/
#define MAX_VA_STRING 8192

IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
char *va(const char *fmt, ...)
{
    va_list argptr;

    static char buf[MAX_VA_STRING];
    static char str[MAX_VA_STRING]; /* in case va is called by nested funcs */
    static size_t index = 0;

    va_start(argptr, fmt);
    vsnprintf(buf, sizeof(buf), fmt, argptr);
    va_end(argptr);

    size_t len = strnlen(buf, MAX_VA_STRING - 1);
    if (len + index >= MAX_VA_STRING - 1)
        index = 0;

    char *ptr = &str[index];
    memcpy(ptr, buf, len + 1);
    index += len + 1;

    return ptr;
}
