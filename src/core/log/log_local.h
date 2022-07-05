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

#ifndef LOG_LOCAL_H
#define LOG_LOCAL_H

#include "iocod.h"

/**
 * @defgroup log_local Logging (local)
 * @brief Local config procedures and structures needed for log module.
 * @{
 */

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#else
#include <pthread.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <time.h>

#define LOG_MAX_PRINT     4096 /* maximum characters that can be printed */
#define LOG_MAX_TIMESTAMP 64    /* maximum timestamp length */

/*
 * Log structure.
 */
struct logger {
    char name[64];          /**< name of log, currently unused */
    char path[256];         /**< path to log file */

    enum log_level level;   /**< log level */

    bool echo_stdout;       /**< echo messages to stdout */
    bool auto_lf;           /**< automatically add lf to output */
    bool hide_next_source;  /**< hide next message's source */

    FILE *fp;               /**< file */
    size_t size;            /**< size of bytes written */

    size_t last_flush;

    struct timeval now;     /**< current time */
};

extern struct logger iclog;

#ifdef IC_PLATFORM_WINDOWS
extern CRITICAL_SECTION log_mutex;
#else
extern pthread_mutex_t log_mutex;
#endif

/*
 * These cross-platform time utilities from https://github.com/yksz/c-logger
 */
#ifdef IC_PLATFORM_WINDOWS
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

int gettimeofday(struct timeval *tv, struct timezone *tz);
struct tm *localtime_r(const time_t *timep, struct tm *result);
#endif

/**
 * @brief Get string timestamp of current time.
 * @param time
 * @param timestamp
 * @param size
*/
void log_get_timestamp(const struct timeval *time, char *timestamp,
                       size_t size);

/**
 * @brief Lock the mutex for thread safety.
 */
void log_lock(void);

/**
 * @brief Unlock the mutex.
 */
void log_unlock(void);

/**
 * @brief Return the current thread id.
 */
intptr_t log_current_thread_id(void);

/** @} */

#endif /* LOG_LOCAL_H */
