/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>

#if defined(_WIN32)
#include <winsock2.h>
#else
#ifdef ENABLE_THREADING
#include <pthread.h>
#endif
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

#include "logger.h"
#include "stringlib.h"

static const char *level_output_str[] = {
    [LOG_LEVEL_NONE] = " ",
    [LOG_LEVEL_FATAL] = "F",
    [LOG_LEVEL_ERROR] = "E",
    [LOG_LEVEL_WARN] = "W",
    [LOG_LEVEL_INFO] = "I",
    [LOG_LEVEL_DEBUG] = "D",
    [LOG_LEVEL_TRACE] = "T",
    [LOG_LEVEL_ALL] = " "
};

static const char *stdlevel_output_str[] = {
    [LOG_LEVEL_NONE] = " ",
    [LOG_LEVEL_FATAL] = "******** FATAL ERROR ******** ",
    [LOG_LEVEL_ERROR] = "**** ERROR **** ",
    [LOG_LEVEL_WARN] = "WARNING: ",
    [LOG_LEVEL_INFO] = "info: ",
    [LOG_LEVEL_DEBUG] = " ",
    [LOG_LEVEL_TRACE] = " ",
    [LOG_LEVEL_ALL] = " "
};

static const char *color_output_str[] = {
    [LOG_LEVEL_NONE] = "",
    [LOG_LEVEL_FATAL] = LOG_COLOR_FATAL,
    [LOG_LEVEL_ERROR] = LOG_COLOR_ERROR,
    [LOG_LEVEL_WARN] = LOG_COLOR_WARN,
    [LOG_LEVEL_INFO] = LOG_COLOR_INFO,
    [LOG_LEVEL_DEBUG] = LOG_COLOR_DEBUG,
    [LOG_LEVEL_TRACE] = LOG_COLOR_TRACE,
    [LOG_LEVEL_ALL] = ""
};

/**
 * @author yksz (https://github.com/yksz/c-logger)
*/
static void lock(struct logger *l)
{
    #if defined(ENABLE_THREADING)
    #if defined(_WIN32)
    EnterCriticalSection(&l->mutex);
    #else
    pthread_mutex_lock(&l->mutex);
    #endif /* defined(_WIN32) */
    #endif /* defined(ENABLE_THREADING) */
}

/**
 * @author yksz (https://github.com/yksz/c-logger)
*/
static void unlock(struct logger *l)
{
    #if defined(ENABLE_THREADING)
    #if defined(_WIN32)
    LeaveCriticalSection(&l->mutex);
    #else
    pthread_mutex_unlock(&l->mutex);
    #endif /* defined(_WIN32) */
    #endif /* defined(ENABLE_THREADING) */
}

/**
 * @author yksz (https://github.com/yksz/c-logger)
*/
static long get_current_thread_id(void)
{
    #if defined(_WIN32)
    return GetCurrentThreadId();
    #else
    return syscall(SYS_gettid);
    #endif
}

#if defined(_WIN32)
/**
 * @author yksz (https://github.com/yksz/c-logger)
*/
static int gettimeofday(struct timeval *tv, void *tz)
{
    const UINT64 epochFileTime = 116444736000000000ULL;
    FILETIME ft;
    ULARGE_INTEGER li;
    UINT64 t;

    if (tv == NULL) {
        return -1;
    }
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
static struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    localtime_s(result, timep);
    return result;
}
#endif

/**
 * @author yksz (https://github.com/yksz/c-logger)
*/
static void get_timestamp(const struct timeval *time, char *timestamp,
                          size_t size)
{
    time_t sec = time->tv_sec;
    struct tm calendar;

    localtime_r(&sec, &calendar);
    strftime(timestamp, size, "%Y-%m-%d %H:%M:%S", &calendar);
    sprintf(&timestamp[19], ".%06ld", (long) time->tv_usec);
}

/**
 * @brief Init a log file structure 
*/
void log_init(struct logger *l, const char *name, const char *path,
              enum log_level level, enum log_options options)
{
    if (l == NULL) {
        l_error("couldn't allocate memory for logger");
        return;
    }

    if (name == NULL || *name == '\0') {
        l_error("can't instantiate logger with NULL name");
        return;
    }

    if (path == NULL || *path == '\0') {
        l_error("can't instantiate logger with NULL path");
        return;
    }

    memset(l, 0, sizeof(*l));

    strncpy(l->name, name, sizeof(l->name));
    strncpy(l->path, path, sizeof(l->path));
    l->level = level; 
    l->options = options;

    l->fp = NULL;
    l->lasterr = 0;

    // https://github.com/yksz/c-logger
    #if defined(ENABLE_THREADING)
    #if defined(_WIN32)
    InitializeCriticalSection(&l->mutex);
    #else
    if (pthread_mutex_init(&l->mutex, NULL) != 0) {
        l_error("mutex init failed");
        return;
    }
    #endif /* defined(_WIN32) */
    #endif /* defined(ENABLE_THREADING) */

    if (options & LOG_OPT_AUTO_OPEN)
        log_open(l);
}

/**
 * @brief Open a log file
*/
enum log_error log_open(struct logger *l)
{
    if (l == NULL)
        return LOG_ERR_NULL_LOG;

    l->fp = fopen(l->path, "ab");
    if (l->fp == NULL) {
        l->lasterr = errno;

        l_error("couldn't open log file %s: %s", l->path, strerror(errno));
        return LOG_ERR_COULDNT_OPEN;
    }

    return LOG_ERR_OK;
}

/**
 * @brief Close the log with optional closing message
*/
void log_close(struct logger *l)
{
    if (l == NULL || l->fp == NULL)
        return;

    fclose(l->fp);
}

/**
 * @brief Clear a log file by reopening it as write instead of append
*/
void log_clear(struct logger *l)
{
    if (l == NULL || l->fp == NULL)
        return;

    l->fp = freopen(l->path, "wb", l->fp);
    if (l->fp == NULL) {
        l->lasterr = errno;

        l_error("couldn't reopen log file %s: %s", l->path, strerror(errno));
    }
}

void log_disable_stdout(struct logger *l)
{
    if (l == NULL)
        return;

    l->options &= ~LOG_OPT_ECHO_STDOUT;
}

void log_enable_stdout(struct logger *l)
{
    if (l == NULL)
        return;

    l->options |= LOG_OPT_ECHO_STDOUT;
}

/**
 * @brief Print a message to the log and or stdout
*/
void log_print_log(struct logger *l, enum log_level level, const char *function,
                   const char *filename, int line, const char *fmt, ...)
{
    va_list farg, carg;
    char msg[LOG_MAX_PRINT];
    char timestamp[LOG_MAX_TIMESTAMP];
    long threadid;
    int size;
 
    if (l == NULL || l->fp == NULL)
        return;

    // allow LOGLEVEL_ALL prints to always print
    // otherwise, if this message's level is more verbose than our log level
    // ignore it
    if (level != LOG_LEVEL_ALL && level > l->level)
        return;

    gettimeofday(&l->now, NULL);
    get_timestamp(&l->now, timestamp, sizeof(timestamp));
    threadid = get_current_thread_id();

    lock(l);

    // https://github.com/yksz/c-logger
    // level date   time        threadid file:function:line:  message
    // F 2022-03-15 21:05:10.395830 1651 logger.c:main:262: this is a fatal message
    if ((size = fprintf(l->fp, "%s %s %ld %s:%s:%d: ", 
                        level_output_str[level], timestamp, threadid, filename,
                        function, line)) > 0) {
        l->file_size += size;
    }

    // file message
    va_start(farg, fmt);
    if ((size = vfprintf(l->fp, fmt, farg)) > 0)
        l->file_size += size;
    va_end(farg);

    if ((size = fprintf(l->fp, "\n")) > 0)
        l->file_size += size;

    fflush(l->fp);

    // optional stdout
    if (l->options & LOG_OPT_ECHO_STDOUT) {
        va_start(carg, fmt);
        vsnprintf(msg, sizeof(msg), fmt, carg);
        va_end(carg);

        fprintf(stdout, "%s%s", CSI_START, color_output_str[level]);
        fprintf(stdout, "%s:%s:%d: %s%s", filename, function, line, 
                stdlevel_output_str[level], msg);
        fprintf(stdout, "%s\n", CSI_END);
        fflush(stdout);
    }

    unlock(l);
}
