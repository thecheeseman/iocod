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

#include <stdio.h>
#include "iocod.h"

IC_PUBLIC
IC_PRINTF_FORMAT(3, 4)
IC_NON_NULL(1)
ssize_t snprintfz(char *dest, size_t size, const char *fmt, ...)
{
    #if !defined IC_GNUC_VERSION
    if (dest == NULL || fmt == NULL)
        return -1;
    #endif

    if (size == 0)
        return -1;

    va_list argptr;
    va_start(argptr, fmt);
    ssize_t len = vsnprintf(dest, size, fmt, argptr);
    va_end(argptr);
    dest[size - 1] = '\0';

    return len;
}
