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

static enum system_type systype = SYSTEM_UNKNOWN;

IC_PUBLIC
enum system_type sys_system_type(void)
{
    return systype;
}

IC_PUBLIC
bool sys_is_windows(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    return true;
    #else
    return false;
    #endif
}

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

IC_PUBLIC
void sys_platform_init(void)
{
    DWORD version = GetVersion();
    DWORD major = (DWORD) (LOBYTE(LOWORD(version)));
    DWORD minor = (DWORD) (HIBYTE(LOWORD(version)));

    // realistically we don't really care about the difference
    // between 2000, XP, server 2003, etc... but whatever
    if (major == 5) {
        if (minor == 0)
            systype = SYSTEM_WIN2000;
        else if (minor == 1)
            systype = SYSTEM_WINXP;
        else if (minor == 2)
            systype = SYSTEM_WIN2003;
    } else if (major == 6) {
        if (minor == 0)
            systype = SYSTEM_WINVISTA;
        else if (minor == 1)
            systype = SYSTEM_WIN7;
        else if (minor == 2)
            systype = SYSTEM_WIN8_OR_LATER;
    }
}
#else
IC_PUBLIC
void sys_platform_init(void)
{
    #ifdef IC_PLATFORM_MACOS
    systype = SYSTEM_MACOS;
    #else
    systype = SYSTEM_LINUX;
    #endif
}
#endif
