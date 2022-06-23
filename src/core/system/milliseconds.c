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

static bool timebase_init = false;

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//static int32_t timebase = 0;
static ULARGE_INTEGER timebase;

/*
 * Original Q3 method uses timeGetTime() from the WinMM library, but its 
 * precision is generally at best ~5 ms, depending on the machine (e.g.
 * my machine varies from 2-5 ms precision). This version has at least
 * <1 ms precision and up to 1 us precision
 */
IC_PUBLIC
int32_t sys_milliseconds(void)
{
    if (!timebase_init) {
        FILETIME base;
        GetSystemTimeAsFileTime(&base);

        timebase.LowPart = base.dwLowDateTime;
        timebase.HighPart = base.dwHighDateTime;
        timebase_init = true;
    }

    FILETIME curtime;
    GetSystemTimeAsFileTime(&curtime);

    ULARGE_INTEGER cur;
    cur.LowPart = curtime.dwLowDateTime;
    cur.HighPart = curtime.dwHighDateTime;

    return (int32_t) (cur.QuadPart - timebase.QuadPart) / 10000;
}
#else
#include <time.h>

static struct timespec timebase;

/*
 * I'm not entirely sure the _exact_ precision, but it's far better
 * than on Windows (<1 us precision)
 */
IC_PUBLIC
int32_t sys_milliseconds(void)
{
    if (!timebase_init) {
        clock_gettime(CLOCK_REALTIME, &timebase);
        timebase_init = true;
    }

    struct timespec curtime;
    clock_gettime(CLOCK_REALTIME, &curtime);
    double secs = (curtime.tv_sec - timebase.tv_sec) * 1000;
    double ns = (curtime.tv_nsec - timebase.tv_nsec) / 1000000;
    return (int32_t)(secs + ns);
}
#endif
