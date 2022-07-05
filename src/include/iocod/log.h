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

#ifndef IC_LOG_H
#define IC_LOG_H

#include "iocod/platform.h"
#include "iocod/types.h"

#ifndef __FILENAME__
#ifdef IC_PLATFORM_WINDOWS
#define __FILENAME__ \
    (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif 
#endif

/**
 * @defgroup log Logging
 * @brief Utilities for logging.
 * @{
 */

/**
 * @brief Log level.
*/
enum log_level {
    LOG_LEVEL_NONE,     /**< nothing, won't print anything */
    LOG_LEVEL_FATAL,    /**< only print fatal errors */
    LOG_LEVEL_ERROR,    /**< print all errors */
    LOG_LEVEL_WARN,     /**< print warnings + errors */
    LOG_LEVEL_INFO,     /**< print all standard messages */
    LOG_LEVEL_DEBUG,    /**< print debug messages + everything else */
    LOG_LEVEL_TRACE,    /**< print trace messages */
    LOG_LEVEL_ALL       /**< print everything */
};

/**
 * @brief Initialise log.
 */
IC_PUBLIC
void log_init(void);

/**
 * @brief Shutdown log.
 */
IC_PUBLIC
void log_shutdown(void);

/**
 * @brief Clear log.
 */
IC_PUBLIC
void log_clear(void);

/**
 * @brief Print a banner in the log for separation purposes.
 */
IC_PUBLIC
void log_banner(void);

/**
 * @brief Set the logging level.
 *
 * @param[in] new_level new log level
 */
IC_PUBLIC
void log_set_level(enum log_level new_level);

/**
 * @brief Set if the log should echo to stdout.
 *
 * @param[in] echo set to true if log should print to stdout
 */
IC_PUBLIC
void log_echo_stdout(bool echo);

/**
 * @brief Change if the log should automatically add newlines to messages.
 *
 * @param[in] lf set to true if log should add newlines automatically
*/
IC_PUBLIC
void log_auto_lf(bool lf);

/**
 * @brief Print to log.
 *
 * @param[in] level log level of message
 * @param[in] func  parent function that this is called from
 * @param[in] file  file this call is located in
 * @param[in] line  line this call is on
 * @param[in] fmt   formatted string
 * 
 * @return number of characters printed to the log
 *
 * @note Recommended to use the log_ macros instead of calling this directly.
 * @see log_fatal
 * @see log_error
 * @see log_warn
 * @see log_info
 * @see log_debug
 * @see log_trace
 * @see log_print
 */
IC_PUBLIC
IC_PRINTF_FORMAT(5, 6)
size_t log_lprintf(enum log_level level, const char *func, const char *file,
                   int line, const char *fmt, ...);

/**
 * @def log_trace
 * @brief Print a trace message to the log.
 */
#define log_trace(...) \
    log_lprintf(LOG_LEVEL_TRACE, __func__, __FILENAME__, __LINE__, __VA_ARGS__)

/**
 * @def log_debug
 * @brief Print a debug message to the log.
 */
#define log_debug(...) \
    log_lprintf(LOG_LEVEL_DEBUG, __func__, __FILENAME__, __LINE__, __VA_ARGS__)

/**
 * @def log_info
 * @brief Print an informational message to the log.
 */
#define log_info(...) \
    log_lprintf(LOG_LEVEL_INFO, __func__, __FILENAME__, __LINE__, __VA_ARGS__)

/**
 * @def log_warn
 * @brief Print a warning message to the log.
 */
#define log_warn(...) \
    log_lprintf(LOG_LEVEL_WARN, __func__, __FILENAME__, __LINE__, __VA_ARGS__)

/**
 * @def log_error
 * @brief Print an error message to the log.
 */
#define log_error(...) \
    log_lprintf(LOG_LEVEL_ERROR, __func__, __FILENAME__, __LINE__, __VA_ARGS__)

/**
 * @def log_fatal
 * @brief Print a fatal error message to the log.
 */
#define log_fatal(...) \
    log_lprintf(LOG_LEVEL_FATAL, __func__, __FILENAME__, __LINE__, __VA_ARGS__)

/**
 * @def log_print
 * @brief Print a message to the log. Shows regardless of log level setting.
 */
#define log_print(...) \
    log_lprintf(LOG_LEVEL_ALL, __func__, __FILENAME__, __LINE__, __VA_ARGS__)

/** @} */

#endif /* IC_LOG_H */
