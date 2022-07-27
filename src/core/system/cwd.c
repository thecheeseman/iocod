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

#ifdef IC_PLATFORM_WINDOWS
#include <direct.h>
#else
#include <unistd.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 256
#endif

IC_PUBLIC
IC_RETURNS_STRING
char *sys_cwd(void)
{
    static char cwd[PATH_MAX] = { 0 };
    
    #ifdef IC_PLATFORM_WINDOWS
    if (_getcwd(cwd, sizeof(cwd) - 1) == NULL)
        return NULL;
    #else
    if (getcwd(cwd, sizeof(cwd) - 1) == NULL)
        return NULL;
    #endif

    cwd[PATH_MAX - 1] = '\0';
    
    return cwd;
}

