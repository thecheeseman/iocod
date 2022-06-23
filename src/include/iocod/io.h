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

#ifndef IOCOD_IO_H
#define IOCOD_IO_H

/**
 * @brief Printf wrapper that allows direct output to `stderr`.
 *
 * If `SYS_PRINT` or `SYS_WARN` macros are defined, will call either
 * of those instead.
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
IC_PRINTF_FORMAT(2, 3)
void _ic_print(bool warning, const char *fmt, ...);

/**
 * @def ic_printf
 * @brief Wrapper for `_ic_print`.
 * @see _ic_print
 */
#define ic_printf(...) _ic_print(false, __VA_ARGS__)

/**
 * @def ic_warning
 * @brief Wrapper for `_ic_print`, prepending the message passed with "WARNING".
 * @see _ic_print
 */
#define ic_warning(...) _ic_print(true, __VA_ARGS__)

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
void _ic_error(bool fatal, const char *filename, const char *function,
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
    _ic_error(false, __BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * @def ic_error_fatal
 * @brief Wrapper for _ic_error.
 *
 * Automatically inputs current filename, function name, line number for
 * better error messages. Causes program to exit.
 *
 * @see _ic_error
 */
#define ic_error_fatal(...) \
    _ic_error(true, __BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

#endif /* IOCOD_IO_H */
