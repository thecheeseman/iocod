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

#include "logger.h"

#ifndef _WIN32
#include <sys/syscall.h>
#endif

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
 * @author yksz (https://github.com/yksz/c-logger)
*/
static char get_level_char(enum log_level level)
{
	switch (level) {
	case LOGLEVEL_FATAL: return 'F';
	case LOGLEVEL_ERROR: return 'E';
	case LOGLEVEL_WARN: return 'W';
	case LOGLEVEL_INFO: return 'I';
	case LOGLEVEL_DEBUG: return 'D';
	case LOGLEVEL_TRACE: return 'T';
	case LOGLEVEL_NONE:
	case LOGLEVEL_ALL:
	default:
		break;
	}

	return ' ';
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

	if (options & LOGOPT_AUTO_OPEN)
		log_open(l);
}

/**
 * @brief Open a log file
*/
enum log_error log_open(struct logger *l)
{
	if (l == NULL)
		return LOGERR_NULL_LOG;

	l->fp = fopen(l->path, "ab");
	if (l->fp == NULL) {
		l->lasterr = errno;

		l_error("couldn't open log file %s: %s", l->path, strerror(errno));
		return LOGERR_COULDNT_OPEN;
	}

	return LOGERR_OK;
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
	char levelc;
	const char *flag;

	if (l == NULL || l->fp == NULL)
		return;

	// allow LOGLEVEL_ALL prints to always print
	// otherwise, if this message's level is more verbose than our log level
	// ignore it
	if (level != LOGLEVEL_ALL && level > l->level)
		return;

	gettimeofday(&l->now, NULL);
	get_timestamp(&l->now, timestamp, sizeof(timestamp));
	levelc = get_level_char(level);
	threadid = get_current_thread_id();

	lock(l);

	// https://github.com/yksz/c-logger
	// level date   time        threadid file:function:line:  message
	// F 2022-03-15 21:05:10.395830 1651 logger.c:main:262: this is a fatal message
	if ((size = fprintf(l->fp, "%c %s %ld %s:%s:%d: ", 
						levelc, timestamp, threadid, filename, 
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
	if (l->options & LOGOPT_ECHO_STDOUT) {
		va_start(carg, fmt);
		vsnprintf(msg, sizeof(msg), fmt, carg);
		va_end(carg);

		switch (level) {
		case LOGLEVEL_FATAL: flag = "**** FATAL **** "; break;
		case LOGLEVEL_ERROR: flag = "**** ERROR **** "; break;
		case LOGLEVEL_WARN: flag = "WARNING: "; break;
		case LOGLEVEL_INFO: flag = "info: "; break;
		case LOGLEVEL_DEBUG:
		case LOGLEVEL_TRACE:
		case LOGLEVEL_NONE:
		case LOGLEVEL_ALL:
		default: 
			flag = "";
			break;
		}

		fprintf(stdout, "%s:%s:%d: %s%s\n", filename, function, line, flag, msg);
		fflush(stdout);
	}

	unlock(l);
}

int main(int argc, char *argv[])
{
	(void) argc; (void) argv;

	struct logger log;

	log_init(&log, "test", "test.log", LOGLEVEL_ERROR, LOGOPT_DEFAULT);

	log_trace(&log, "this is a trace message");
	log_debug(&log, "this is a debug print message");
	log_info(&log, "this is a normal info message");
	log_warn(&log, "this is a warning message");
	log_error(&log, "this is an error message");
	log_fatal(&log, "this is a fatal message");

	log_print(&log, "test");

	log_close(&log);

	return 0;
}
