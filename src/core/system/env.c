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

IC_PUBLIC
bool sys_setenv(const char *name, const char *value)
{
    #ifdef IC_PLATFORM_WINDOWS
    if (value != NULL && *value != '\0')
        return (_putenv(va("%s=%s", name, value)) == 0);
    else
        return (_putenv(va("%s=", name)) == 0);
    #else
    if (value != NULL && *value != '\0')
        return (setenv(name, value, 1) == 0);
    else
        return (unsetenv(name) == 0);
    #endif
}

IC_PUBLIC
char *sys_getenv(const char *name)
{
    return getenv(name);
}
