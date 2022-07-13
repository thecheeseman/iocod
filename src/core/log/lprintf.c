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
    [LOG_LEVEL_DEBUG] = "",
    [LOG_LEVEL_TRACE] = "",
    [LOG_LEVEL_ALL] = ""
};

/* color prefixes */
static const char *color_prefix[] = {
    [LOG_LEVEL_NONE] = "^7",
    [LOG_LEVEL_FATAL] = "^1",
    [LOG_LEVEL_ERROR] = "^1",
    [LOG_LEVEL_WARN] = "^3",
    [LOG_LEVEL_INFO] = "^7",
    [LOG_LEVEL_DEBUG] = "^5",
    [LOG_LEVEL_TRACE] = "^4",
    [LOG_LEVEL_ALL] = "^7"
};

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
    log_get_timestamp(&iclog.now, timestamp, sizeof(timestamp));

    intptr_t threadid = (intptr_t) thread_current_thread_id();
    thread_mutex_lock(&log_mutex);

    /*
        https://github.com/yksz/c-logger
        level date   time        threadid file:function:line:  message
        F 2022-03-15 21:05:10.395830 1651 logger.c:main:262: this is a fatal message
    */
    size_t size = 0;
    size_t printed = 0;

    if (!iclog.hide_next_source) {
        if ((size = fprintf(iclog.fp, "%s %s %ld %s:%s:%d: ",
                            log_prefix[level], timestamp, (long) threadid, file,
                            func, line)) > 0) {
            iclog.size += size;
            printed += size;
        }
    } else {
        iclog.hide_next_source = false;
    }

    /* handle message */
    char msg[LOG_MAX_PRINT];
    va_list argptr = { 0 };
    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    bool has_newline = (msg[strlen(msg) - 1] == '\n');

    if ((size = fprintf(iclog.fp, "%s%s", msg,
                        (!has_newline && iclog.auto_lf) 
                        ? IC_PLATFORM_NEWLINE : "")) > 0) {
        iclog.size += size;
        printed += size;
    }

    // if we're not buffered, force flush now
    // otherwise if this is an error, always force flush to file
    if (!iclog.buffered || 
        level == LOG_LEVEL_ERROR || level == LOG_LEVEL_FATAL) {
        iclog.last_flush = iclog.size;
        fflush(iclog.fp);
    }

    const char *color = color_prefix[level];
    #ifndef IC_PLATFORM_WINDOWS
    if (!config_console_colors())
        color = "";
    #endif

    /* echo to stdout */
    if (iclog.echo_stdout) {
        con_print(va("%s%s%s%s", 
                     color,
                     stdout_prefix[level], 
                     msg,
                     (!has_newline && iclog.auto_lf)
                     ? "\n" : "")); // IC_PLATFORM_NEWLINE not necessary here
                                    // con_print() will replace \n with it
    }

    thread_mutex_unlock(&log_mutex);

    return printed;
}
