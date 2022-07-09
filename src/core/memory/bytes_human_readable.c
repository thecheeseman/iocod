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

#include "memory_local.h"

char *bytes_human_readable(size_t size)
{
    static char str[64];

    if (size < 1024)
        snprintf(str, sizeof(str), "%zu bytes", size);
    else if (size < 1048576)
        snprintf(str, sizeof(str), "%4.2f KB", BTOKB(size));
    else if (size < 1073741824)
        snprintf(str, sizeof(str), "%4.2f MB", BTOMB(size));
    else
        snprintf(str, sizeof(str), "%4.2f GB", BTOGB(size));

    return str;
}
