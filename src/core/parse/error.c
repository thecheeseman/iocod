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

extern struct parse_info *pi;

IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void ps_script_warning(const char *fmt, ...)
{
    va_list argptr;
    char str[1024];

    va_start(argptr, fmt);
    vsnprintf(str, sizeof(str), fmt, argptr);
    va_end(argptr);

    ic_warning("file '%s' line %lu: %s\n", pi->parse_file, pi->lines, str);
}

IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void ps_script_error(const char *fmt, ...)
{
    va_list argptr;
    char str[1024];

    va_start(argptr, fmt);
    vsnprintf(str, sizeof(str), fmt, argptr);
    va_end(argptr);

    ic_error("file '%s' line %lu: %s\n", pi->parse_file, pi->lines, str);
}
