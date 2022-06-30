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

#ifndef IC_COMMON_H
#define IC_COMMON_H

#include "iocod.h"

#define PROTOCOL_VERSION 10
#define PROTOCOL_LEGACY 1

extern struct cvar *com_busy_wait;
extern struct cvar *com_dedicated;
extern struct cvar *com_developer;
extern struct cvar *com_sv_running;
extern struct cvar *com_speeds;

IC_PUBLIC
void com_init(char *cmdline);

IC_PUBLIC
int com_milliseconds(void);

IC_PUBLIC
void com_shutdown(void);

IC_PUBLIC
void com_frame(void);

IC_PUBLIC
bool com_filter(char *filter, char *name, bool casesensitive);

IC_PUBLIC
bool com_filter_path(char *filter, char *name, bool casesensitive);

/**
 * @brief Check if the given string @p str1 contains @p str2.
 * @param[in] str1 the string to check.
 * @param[in] str2 the string to check for.
 * @param[in] casesensitive if true, the comparison is case sensitive.
 * @return NULL if @p str1 does not contain @p str2, otherwise a
 * pointer to the last matching character of the string
*/
IC_PUBLIC
char *com_string_contains(char *str1, char *str2, bool casesensitive);

/**
 * @brief Printf wrapper that allows direct output to console.
 *
 * @note It is recommended to access this function via the macros
 * `ic_printf()` and `ic_warning()` instead of calling this directly.
 *
 * @param[in] warning true if this is warning, false otherwise
 * @param[in] fmt     format specifier
 * @param[in] ...
 *
 * @see ic_printf
 * @see ic_warning
 */
IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void ic_printf(const char *fmt, ...);

#define ic_warning(...) ic_printf(__VA_ARGS__)

IC_PUBLIC
void ic_print_header(const char *text, size_t size, char sep);

enum errcode {
    ERR_FATAL,
    ERR_DROP,
    ERR_SERVER_DISCONNECT,
    ERR_DISCONNECT,
    ERR_NEED_CD
};

/**
 * @brief Error wrapper that allows direct output to `stderr`.
 *
 * If `COM_ERROR` is defined, will call that instead.
 *
 * @note It is recommended to access this function via the macros
 * `ic_error()` and `ic_error_fatal()` instead of calling this directly.
 *
 * @param[in] fatal    true if this is a fatal error and we should exit
 * @param[in] filename name of file this was called from
 * @param[in] function name of function this was called from
 * @param[in] line     line number
 * @param[in] fmt      format specifier
 * @param[in] ...
 *
 * @see ic_error
 * @see ic_error_fatal
 */
IC_PUBLIC
IC_PRINTF_FORMAT(5, 6)
void _ic_error(enum errcode code, const char *filename, const char *function,
               const int line, const char *fmt, ...);

/**
 * @def ic_error
 * @brief Wrapper for _ic_error.
 *
 * Automatically inputs current filename, function name, line number for
 * better error messages.
 *
 * @see _ic_error
 */
#define ic_error(...) \
    _ic_error(ERR_DROP, __BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * @def ic_fatal
 * @brief Wrapper for _ic_error.
 *
 * Automatically inputs current filename, function name, line number for
 * better error messages. Causes program to exit.
 *
 * @see _ic_error
 */
#define ic_fatal(...) \
    _ic_error(ERR_FATAL, __BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

#endif /* IC_COMMON_H */
