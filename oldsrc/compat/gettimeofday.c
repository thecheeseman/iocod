#include "compat/gettimeofday.h"

#ifdef _WIN32
/**
 * @author yksz (https://github.com/yksz/c-logger)
*/
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    const UINT64 epochFileTime = 116444736000000000ULL;
    FILETIME ft;
    ULARGE_INTEGER li;
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

/**
 * @author yksz (https://github.com/yksz/c-logger)
*/
struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    localtime_s(result, timep);
    return result;
}
#endif
