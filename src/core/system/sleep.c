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
#include <sys/select.h>
#endif

IC_PUBLIC
void sys_sleep(int msec)
{
    if (msec == 0)
        return;

    #ifdef IC_PLATFORM_WINDOWS
    if (msec < 0)
        WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), INFINITE);
    else
        WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), msec);
    #else
    if (stdin_is_atty()) {
        fd_set fdset = { 0 };
        FD_ZERO(&fdset);
        FD_SET(STDIN_FILENO, &fdset);

        if (msec < 0) {
            select(STDIN_FILENO + 1, &fdset, NULL, NULL, NULL);
        } else {
            struct timeval timeout = { 0 };
            timeout.tv_sec = msec / 1000;
            timeout.tv_usec = (msec % 1000) * 1000;
            select(STDIN_FILENO + 1, &fdset, NULL, NULL, &timeout);
        }
    } else {
        if (msec < 0)
            msec = 10;

        usleep(msec * 1000);
    }
    #endif
}
