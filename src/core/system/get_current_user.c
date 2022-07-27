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
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif

IC_PUBLIC
IC_RETURNS_STRING
char *sys_get_current_user(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    static wchar_t wusername[256] = { 0 };
    static char username[256] = { 0 };
    unsigned long size = sizeof(wusername);

    GetUserNameW(wusername, &size);
    utf16_shorten(wusername, username);

    if (username[0] == '\0')
        strncpy(username, "player", sizeof(username));

    return username;
    #else
    struct passwd *pw;

    if ((pw = getpwuid(getuid())) == NULL)
        return "player";

    return pw->pw_name;
    #endif
}
