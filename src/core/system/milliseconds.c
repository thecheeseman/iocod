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

#include "iocod/platform.h"
#include "iocod/types.h"
#include <inttypes.h>

static bool timebase_init = false;

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static LARGE_INTEGER frequency;
static LARGE_INTEGER timebase;

IC_PUBLIC
int32_t sys_milliseconds(void)
{
    if (!timebase_init) {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&timebase);
        timebase_init = true;
    }
    
    LARGE_INTEGER now = { 0 };
    QueryPerformanceCounter(&now);

    LARGE_INTEGER elapsed = { 0 };
    elapsed.QuadPart = now.QuadPart - timebase.QuadPart;
    elapsed.QuadPart *= 1000000;
    elapsed.QuadPart /= frequency.QuadPart;

    return (int32_t) (elapsed.QuadPart / 1000);
}
#else
#include <time.h>

static struct timespec timebase;

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
    return (int32_t) (secs + ns);
}
#endif
