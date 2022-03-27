#ifndef COMPAT_GETTIMEOFDAY_H
#define COMPAT_GETTIMEOFDAY_H

#ifdef _WIN32
#include <time.h>
#include <winsock.h>

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

extern int gettimeofday(struct timeval *tv, struct timezone *tz);
extern struct tm *localtime_r(const time_t *timep, struct tm *result);
#endif

#endif /* COMPAT_GETTIMEOFDAY_H */
