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

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>

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

#define l_malloc malloc
#define l_free free
#define l_error(...) fprintf(stderr, __VA_ARGS__)

#define LOG_MAX_PRINT 16384
#define LOG_MAX_TIMESTAMP 64

// https://github.com/yksz/c-logger
#if defined(_WIN32)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif /* defined(_WIN32) */

enum log_level {
	LOGLEVEL_NONE,
	LOGLEVEL_FATAL,
	LOGLEVEL_ERROR,
	LOGLEVEL_WARN,
	LOGLEVEL_INFO,
	LOGLEVEL_DEBUG,
	LOGLEVEL_TRACE,
	LOGLEVEL_ALL
};

enum log_error {
	LOGERR_OK,
	LOGERR_NULL_LOG,
	LOGERR_COULDNT_OPEN
};

enum log_options {
	LOGOPT_NONE = 0,
	LOGOPT_ECHO_STDOUT = 1,
	LOGOPT_AUTO_OPEN = 2
};

#define LOGOPT_DEFAULT (LOGOPT_AUTO_OPEN | LOGOPT_ECHO_STDOUT)

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
    log_print_log(l, LOGLEVEL_TRACE, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_debug(l, ...) \
    log_print_log(l, LOGLEVEL_DEBUG, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_info(l, ...) \
    log_print_log(l, LOGLEVEL_INFO, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_warn(l, ...) \
    log_print_log(l, LOGLEVEL_WARN, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_error(l, ...) \
    log_print_log(l, LOGLEVEL_ERROR, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_fatal(l, ...) \
    log_print_log(l, LOGLEVEL_FATAL, __func__, __FILENAME__, __LINE__, __VA_ARGS__)
#define log_print(l, ...) \
    log_print_log(l, LOGLEVEL_ALL, __func__, __FILENAME__, __LINE__, __VA_ARGS__)

#endif // __LOGGER_H__
