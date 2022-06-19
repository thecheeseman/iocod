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

/**
 * @file   log.c
 * @author thecheeseman
 * @date   2022-05-14
 *
 * Logging functionality.
 */

#include <errno.h>
#include <stdio.h>
#include <time.h>

#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#else
#include <pthread.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

#define LOG_MAX_PRINT     4096 /* maximum characters that can be printed */
#define LOG_MAX_TIMESTAMP 64    /* maximum timestamp length */

/* https://github.com/armink/EasyLogger/blob/master/easylogger/src/elog.c */
#define CSI_START                       "\033["
#define CSI_END                         "\033[0m"
/* output log front color */
#define LFF_BLACK                       "30;"
#define LFF_RED                         "31;"
#define LFF_GREEN                       "32;"
#define LFF_YELLOW                      "33;"
#define LFF_BLUE                        "34;"
#define LFF_MAGENTA                     "35;"
#define LFF_CYAN                        "36;"
#define LFF_WHITE                       "37;"
/* output log background color */
#define LFB_NULL                        ""
#define LFB_BLACK                       "40;"
#define LFB_RED                         "41;"
#define LFB_GREEN                       "42;"
#define LFB_YELLOW                      "43;"
#define LFB_BLUE                        "44;"
#define LFB_MAGENTA                     "45;"
#define LFB_CYAN                        "46;"
#define LFB_WHITE                       "47;"
/* output log fonts style */
#define LFS_BOLD                        "1m"
#define LFS_UNDERLINE                   "4m"
#define LFS_BLINK                       "5m"
#define LFS_NORMAL                      "22m"
/* output log default color definition: [front color] + [background color] + [show style] */
#define LOG_COLOR_FATAL                 (LFF_WHITE   LFB_RED  LFS_NORMAL)
#define LOG_COLOR_ERROR                 (LFF_RED     LFB_NULL LFS_NORMAL)
#define LOG_COLOR_WARN                  (LFF_YELLOW  LFB_NULL LFS_NORMAL)
#define LOG_COLOR_INFO                  (LFF_WHITE   LFB_NULL LFS_NORMAL)
#define LOG_COLOR_DEBUG                 (LFF_CYAN    LFB_NULL LFS_NORMAL)
#define LOG_COLOR_TRACE                 (LFF_MAGENTA LFB_NULL LFS_NORMAL)
#define LOG_COLOR_PRINT                 (LFF_WHITE   LFB_NULL LFS_NORMAL)

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
    bool enable_color;      /**< enable CSI colors */

    FILE *fp;               /**< file */
    size_t size;            /**< size of bytes written */

    struct timeval now;     /**< current time */
};

static struct logger iclog = {
    .name = "iocod",
    .path = "iocod.log",
    .level = LOG_LEVEL_INFO,
    .echo_stdout = true,
    .auto_lf = true,
    .hide_next_source = false,
    .enable_color = false,
    .size = 0
};

/* string representation of log level */
static const char *level_str[] = {
    [LOG_LEVEL_NONE] = "none",
    [LOG_LEVEL_FATAL] = "fatal",
    [LOG_LEVEL_ERROR] = "error",
    [LOG_LEVEL_WARN] = "warn",
    [LOG_LEVEL_INFO] = "info",
    [LOG_LEVEL_DEBUG] = "debug",
    [LOG_LEVEL_TRACE] = "trace",
    [LOG_LEVEL_ALL] = "all"
};

/* log level prefix */
static const char *log_prefix[] = {
    [LOG_LEVEL_NONE] = " ",
    [LOG_LEVEL_FATAL] = "F",
    [LOG_LEVEL_ERROR] = "E",
    [LOG_LEVEL_WARN] = "W",
    [LOG_LEVEL_INFO] = "I",
    [LOG_LEVEL_DEBUG] = "D",
    [LOG_LEVEL_TRACE] = "T",
    [LOG_LEVEL_ALL] = " "
};

/* stdout prefixes */
static const char *stdout_prefix[] = {
    [LOG_LEVEL_NONE] = "",
    [LOG_LEVEL_FATAL] = "FATAL ERROR: ",
    [LOG_LEVEL_ERROR] = "ERROR: ",
    [LOG_LEVEL_WARN] = "WARNING: ",
    [LOG_LEVEL_INFO] = "",
    [LOG_LEVEL_DEBUG] = "debug: ",
    [LOG_LEVEL_TRACE] = "trace: ",
    [LOG_LEVEL_ALL] = ""
};

/* color prefixes */

static const char *color_prefix[] = {
    [LOG_LEVEL_NONE] = "",
    [LOG_LEVEL_FATAL] = LOG_COLOR_FATAL,
    [LOG_LEVEL_ERROR] = LOG_COLOR_ERROR,
    [LOG_LEVEL_WARN] = LOG_COLOR_WARN,
    [LOG_LEVEL_INFO] = LOG_COLOR_INFO,
    [LOG_LEVEL_DEBUG] = LOG_COLOR_DEBUG,
    [LOG_LEVEL_TRACE] = LOG_COLOR_TRACE,
    [LOG_LEVEL_ALL] = LOG_COLOR_PRINT
};

#ifdef IC_PLATFORM_WINDOWS
static CRITICAL_SECTION log_mutex;
#else
static pthread_mutex_t log_mutex;
#endif

/*
 * Lock the mutex for thread safety.
 */
static void lock(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    EnterCriticalSection(&log_mutex);
    #else
    pthread_mutex_lock(&log_mutex);
    #endif
}

/* 
 * Unlock the mutex.
 */
static void unlock(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    LeaveCriticalSection(&log_mutex);
    #else
    pthread_mutex_unlock(&log_mutex);
    #endif
}

/*
 * Return the current thread id.
 */
static intptr_t current_thread_id(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    return (intptr_t) GetCurrentThreadId();
    #else
    return (intptr_t) pthread_self();
    #endif
}

/*
 * These cross-platform time utilities from https://github.com/yksz/c-logger
 */
#ifdef IC_PLATFORM_WINDOWS
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

static int gettimeofday(struct timeval *tv, struct timezone *tz)
{
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

static struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    localtime_s(result, timep);
    return result;
}
#endif

static void get_timestamp(const struct timeval *time, char *timestamp,
                          size_t size)
{
    time_t sec = time->tv_sec;
    struct tm calendar;

    localtime_r(&sec, &calendar);
    strftime(timestamp, size, "%Y-%m-%d %H:%M:%S", &calendar);
    sprintf(&timestamp[19], ".%06ld", (long) time->tv_usec);
}

/*
 * Log init.
 */
IC_PUBLIC
void log_init(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    InitializeCriticalSection(&log_mutex);
    #else
    if (pthread_mutex_init(&log_mutex, NULL) != 0) {
        ic_error(_("Mutex init failed\n"));
        return;
    }
    #endif

    iclog.fp = fopen(iclog.path, "ab");
    if (iclog.fp == NULL) {
        ic_error(_("Couldn't open logfile '%1$s': %2$s\n"), 
                 iclog.path, strerror(errno));
        return;
    }

    /* get current file size */
    long pos = ftell(iclog.fp);
    fseek(iclog.fp, 0, SEEK_END);
    size_t end = ftell(iclog.fp);
    fseek(iclog.fp, pos, SEEK_SET);
    iclog.size = end;

    /* TODO: file splitting once file gets too large? */

    log_banner();
    log_debug(_("Log file opened\n"));
}

/*
 * Log shutdown.
 */
IC_PUBLIC
void log_shutdown(void)
{
    log_debug(_("Log file closed\n"));
    log_banner();

    fclose(iclog.fp);
    iclog.fp = NULL;

    #ifdef IC_PLATFORM_WINDOWS
    DeleteCriticalSection(&log_mutex);
    #else
    pthread_mutex_destroy(&log_mutex);
    #endif
}

/*
 * Log clear.
 */
IC_PUBLIC
void log_clear(void)
{
    if (iclog.fp == NULL)
        return;

    iclog.fp = freopen(iclog.path, "wb", iclog.fp);
    if (iclog.fp == NULL) {
        ic_error(_("Couldn't open logfile '%1$s': %2$s\n"), 
                 iclog.path, strerror(errno));
        return;
    }

    log_banner();
    log_debug(_("Log cleared\n"));
}

/*
 * Print a banner.
 */
IC_PUBLIC
void log_banner(void)
{
    bool oldstdout = iclog.echo_stdout;
    iclog.echo_stdout = false;

    iclog.hide_next_source = true;
    log_print("--------------------------------------------------------------------------------");

    iclog.echo_stdout = oldstdout;
}

/*
 * Set log level.
 */
IC_PUBLIC
void log_set_level(enum log_level new_level)
{
    IC_ASSERT((new_level >= LOG_LEVEL_NONE) && (new_level <= LOG_LEVEL_ALL));

    if (iclog.level != new_level) {
        log_debug(_("Log level changed from '%1$s' to '%2$s'\n"),
                  level_str[iclog.level], level_str[new_level]);

        iclog.level = new_level;
    }
}

/*
 * Set log stdout.
 */
IC_PUBLIC
void log_echo_stdout(bool echo)
{
    IC_ASSERT((echo == true) || (echo == false));

    if (iclog.echo_stdout != echo) {
        log_debug(_("log option 'echo_stdout' changed to '%s'\n"),
                  echo ? _("true") : _("false"));

        iclog.echo_stdout = echo;
    }
}

/*
 * Set auto lf.
 */
IC_PUBLIC
void log_auto_lf(bool lf)
{
    IC_ASSERT((lf == true) || (lf == false));

    if (iclog.auto_lf != lf) {
        log_debug(_("log option 'auto_lf' changed to '%s'\n"),
                  lf ? _("true") : _("false"));

        iclog.auto_lf = lf;
    }
}

/*
 * Set color.
 */
IC_PUBLIC
void log_enable_color(bool color)
{
    IC_ASSERT((color == true) || (color == false));

    if (iclog.enable_color != color) {
        iclog.enable_color = color;

        log_debug(_("log option 'enable_color' changed to '%s'\n"),
                  color ? _("true") : _("false"));
    }
}

/*
 * Log print.
 */
IC_PUBLIC
IC_PRINTF_FORMAT(5, 6)
size_t log_lprintf(enum log_level level, const char *func, const char *file,
                   int line, const char *fmt, ...)
{
    IC_ASSERT((level >= LOG_LEVEL_NONE) && (level <= LOG_LEVEL_ALL));

    if (iclog.fp == NULL)
        return 0;

    /* ignore message if it's not a high enough priority */
    if (level != LOG_LEVEL_ALL && level > iclog.level)
        return 0;

    gettimeofday(&iclog.now, NULL);

    char timestamp[LOG_MAX_TIMESTAMP];
    get_timestamp(&iclog.now, timestamp, sizeof(timestamp));

    long threadid = current_thread_id();

    lock();

    /* 
        https://github.com/yksz/c-logger
        level date   time        threadid file:function:line:  message
        F 2022-03-15 21:05:10.395830 1651 logger.c:main:262: this is a fatal message
    */
    size_t size;
    size_t printed = 0;

    if (!iclog.hide_next_source) {
        if ((size = fprintf(iclog.fp, "%s %s %ld %s:%s:%d: ",
                            log_prefix[level], timestamp, threadid, file,
                            func, line)) > 0) {
            iclog.size += size;
            printed += size;
        }
    } else {
        iclog.hide_next_source = false;
    }

    /* handle message */
    char msg[LOG_MAX_PRINT];
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    if ((size = fprintf(iclog.fp, "%s", msg)) > 0) {
        iclog.size += size;
        printed += size;
    }

    /* auto lf */
    if (msg[size - 1] != '\n' && iclog.auto_lf) {
        if ((size = fprintf(iclog.fp, "%s", IC_PLATFORM_NEWLINE)) > 0) {
            iclog.size += size;
            printed += size;
        }
    }

    fflush(iclog.fp);

    /* echo to stdout */
    if (iclog.echo_stdout) {
        if (iclog.enable_color)
            fprintf(stdout, "%s%s", CSI_START, color_prefix[level]);

        fprintf(stdout, "%s%s", stdout_prefix[level], msg);

        if (iclog.enable_color)
            fprintf(stdout, "%s", CSI_END);

        if (msg[size - 1] != '\n' && iclog.auto_lf)
            fprintf(stdout, "%s", IC_PLATFORM_NEWLINE);

        fflush(stdout);
    }

    unlock();

    return printed;
}
