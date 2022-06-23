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
