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

#include "log_local.h"

/*
 * These cross-platform time utilities from https://github.com/yksz/c-logger
 */
#ifdef IC_PLATFORM_WINDOWS
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    UNUSED_PARAM(tz);

    const UINT64 epochFileTime = 116444736000000000ULL;
    FILETIME ft;
    ULARGE_INTEGER li = { 0 };
    UINT64 t;

    if (tv == NULL)
        return -1;

    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    t = (li.QuadPart - epochFileTime) / 10;
    tv->tv_sec = (long) (t / 1000000);
    tv->tv_usec = t % 1000000;
    return 0;
}

struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    localtime_s(result, timep);
    return result;
}
#endif

void log_get_timestamp(const struct timeval *time, char *timestamp,
                          size_t size)
{
    time_t sec = time->tv_sec;
    struct tm calendar;

    localtime_r(&sec, &calendar);
    strftime(timestamp, size, "%Y-%m-%d %H:%M:%S", &calendar);
    sprintf(&timestamp[19], ".%06ld", (long) time->tv_usec);
}
