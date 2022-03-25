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

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN32)
#include <winsock2.h>
#else
#ifdef ENABLE_THREADING
#include <pthread.h>
#endif
#endif

#define l_malloc malloc
#define l_free free
#define l_error(...) fprintf(stderr, __VA_ARGS__)

#define LOG_MAX_PRINT 16384
#define LOG_MAX_TIMESTAMP 64

/* https://github.com/yksz/c-logger */
#if defined(_WIN32)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif /* defined(_WIN32) */

/* https://github.com/armink/EasyLogger/blob/master/easylogger/src/elog.c */
#define CSI_START                      "\033["
#define CSI_END                        "\033[0m"
/* output log front color */
#define F_BLACK                        "30;"
#define F_RED                          "31;"
#define F_GREEN                        "32;"
#define F_YELLOW                       "33;"
#define F_BLUE                         "34;"
#define F_MAGENTA                      "35;"
#define F_CYAN                         "36;"
#define F_WHITE                        "37;"
/* output log background color */
#define B_NULL
#define B_BLACK                        "40;"
#define B_RED                          "41;"
#define B_GREEN                        "42;"
#define B_YELLOW                       "43;"
#define B_BLUE                         "44;"
#define B_MAGENTA                      "45;"
#define B_CYAN                         "46;"
#define B_WHITE                        "47;"
/* output log fonts style */
#define S_BOLD                         "1m"
#define S_UNDERLINE                    "4m"
#define S_BLINK                        "5m"
#define S_NORMAL                       "22m"
/* output log default color definition: [front color] + [background color] + [show style] */
#define LOG_COLOR_FATAL               (F_MAGENTA B_NULL S_NORMAL)
#define LOG_COLOR_ERROR               (F_RED B_NULL S_NORMAL)
#define LOG_COLOR_WARN                (F_YELLOW B_NULL S_NORMAL)
#define LOG_COLOR_INFO                (F_CYAN B_NULL S_NORMAL)
#define LOG_COLOR_DEBUG               (F_GREEN B_NULL S_NORMAL)
#define LOG_COLOR_TRACE               (F_BLUE B_NULL S_NORMAL)

enum log_level {
    LOG_LEVEL_NONE,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_ALL
};

enum log_error {
    LOG_ERR_OK,
    LOG_ERR_NULL_LOG,
    LOG_ERR_COULDNT_OPEN
};

enum log_options {
    LOG_OPT_NONE = 0,
    LOG_OPT_ECHO_STDOUT = 1,
    LOG_OPT_AUTO_OPEN = 2
};

#define LOG_OPT_DEFAULT (LOG_OPT_AUTO_OPEN | LOG_OPT_ECHO_STDOUT)

struct logger {
    char name[64];
    char path[256];

    enum log_level level;
    enum log_options options;

    FILE *fp;
    size_t file_size;
    int lasterr;

    struct timeval now;

    // https://github.com/yksz/c-logger
    #if defined(ENABLE_THREADING)
    #if defined(_WIN32)
    CRITICAL_SECTION mutex;
    #else
    pthread_mutex_t mutex;
    #endif /* defined(_WIN32) */
    #endif /* defined(ENABLE_THREADING) */
};

void log_init(struct logger *l, const char *name, const char *path,
              enum log_level level, enum log_options options);
enum log_error log_open(struct logger *l);
void log_close(struct logger *l);
void log_clear(struct logger *l);
void log_disable_stdout(struct logger *l);
void log_enable_stdout(struct logger *l);
void log_print_log(struct logger *l, enum log_level level, const char *function,
                   const char *filename, int line, const char *fmt, ...);

// https://github.com/yksz/c-logger

#define log_trace(l, ...) \
    log_print_log(l, LOG_LEVEL_TRACE, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_debug(l, ...) \
    log_print_log(l, LOG_LEVEL_DEBUG, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_info(l, ...) \
    log_print_log(l, LOG_LEVEL_INFO, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_warn(l, ...) \
    log_print_log(l, LOG_LEVEL_WARN, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_error(l, ...) \
    log_print_log(l, LOG_LEVEL_ERROR, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_fatal(l, ...) \
    log_print_log(l, LOG_LEVEL_FATAL, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_print(l, ...) \
    log_print_log(l, LOG_LEVEL_ALL, __func__, __FILENAME__, __LINE__, __VA_ARGS__)

#endif /* LOGGER_H */
