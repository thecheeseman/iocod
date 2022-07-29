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

#include "fs_local.h"

#ifndef IC_PLATFORM_WINDOWS
#include <sys/stat.h>
#endif

IC_NON_NULL(1, 2)
FILE *fs_fopen(_In_z_ const char *path,
               _In_z_ const char *mode)
{
    #ifdef IC_PLATFORM_WINDOWS
    size_t len = strlen(path);
    if (len == 0 || path[len - 1] == ' ' || path[len - 1] == '.')
        return NULL;
    #else
    struct stat buf;

    if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode))
        return NULL;
    #endif

    return fopen(path, mode);
}
