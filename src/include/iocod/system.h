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

#ifndef IC_SYSTEM_H
#define IC_SYSTEM_H

#include "iocod/platform.h"
#include "iocod/types.h"
#include <inttypes.h>

IC_PUBLIC
void sys_init(void);

IC_PUBLIC
char *sys_cwd(void);

IC_PUBLIC
qbool sys_mkdir(const char *path);

IC_PUBLIC
char *sys_get_current_user(void);

/**
 * @brief Get last library error, if applicable. Clears last error.
 * @return NULL-terminated string containing error message, otherwise NULL
*/
IC_PUBLIC
char *sys_library_error(void);

/**
 * @brief Load a system library.
 * @param[in]  path   file path to library module
 * @param[out] handle handle to loaded library module, NULL if failed
 * @return true if success, false otherwise
*/
IC_PUBLIC
qbool sys_library_load(const char *path, void **handle);

/**
 * @brief Close a system library.
 * @param[in] handle loaded library module
 * @return true if success, false otherwise
*/
IC_PUBLIC
qbool sys_library_close(void *handle);

/**
 * @brief Load a given @p symbol in a library module @p handle
 *
 * @note MSVC will throw warnings if you do not cast @p symbol to (void *)
 *
 * @param[in] handle  loaded library module
 * @param[in] fn      name of symbol to load
 * @param[out] symbol handle to symbol, NULL if failed
 * @return true if success, false otherwise
*/
IC_PUBLIC
qbool sys_library_load_symbol(void *handle, const char *fn, void **symbol);

/**
 * @brief Get current milliseconds since application began.
 * @return current milliseconds
*/
IC_PUBLIC
int32_t sys_milliseconds(void);

/**
 * @brief Cause the system to do a backtrace dump in case of segfaults.
*/
IC_PUBLIC
void sys_backtrace(void);

enum exit_code {
    /**
     * @brief Normal exit.
    */
    IC_OK,

    /**
     * @brief Abnormal exit due to terminate/interrupt signal.
    */
    IC_TERMINATE,

    /**
     * @brief Abnormal exit due to segmentation fault.
    */
    IC_SEGFAULT,

    /**
     * @brief Abnormal exit due to other signal.
    */
    IC_SIGNAL,

    /**
     * @brief Exit due to error.
    */
    IC_ERROR
};

/**
 * @brief Exit with code @p code.
 * @param code exit code
*/
IC_PUBLIC
IC_NO_RETURN 
void sys_exit(int code);

/**
 * @brief Gracefully quit the system with exit code @ref IC_OK.
*/
IC_PUBLIC
void sys_quit(void);

/**
 * @brief Signal handler. We want to handle most useful signals, especially
 * SIGSEGV and SIGINT as those are the most likely.
 * @param signal signal number
*/
IC_PUBLIC
void sys_signal_handler(int signal);

/**
 * @brief Set up signal handling for errors.
*/
IC_PUBLIC
void sys_setup_signal_handler(void);

/**
 * @brief Set processor floating point rounding to nearest.
*/
IC_PUBLIC
void sys_set_floatenv(void);

/**
 * @brief Windows specific function for printing MessageBox errors.
 * @param function
*/
IC_PUBLIC
void sys_handle_error_exit(const char *function);

/**
 * @brief Platform specific initialization. At the moment, this only 
 * detects which Windows version we're running (if we're on Windows).
*/
IC_PUBLIC
void sys_platform_init();

/**
 * @def sys_is_windows
 * @brief Used to check if we're on Windows -- the @ref IC_PLATFORM_WINDOWS
 * macro is enough for most conditional compilation checks, but sometimes 
 * we need a runtime check because some identical code can have different
 * outcomes on Windows.
 */
#ifdef IC_PLATFORM_WINDOWS
#define sys_is_windows() true
#else
#define sys_is_windows() false
#endif

enum system_type {
    /**
     * @brief Generic Linux.
    */
    SYSTEM_LINUX,

    /**
     * @brief Generic MacOS.
    */
    SYSTEM_MACOS,
    
    /**
     * @brief This is only here for prosterity. No one is using Win2000.
    */
    SYSTEM_WIN2000,

    /**
     * @brief Minimum supported Windows version for iocod.
    */
    SYSTEM_WINXP,

    /**
     * @brief Unlikely anyone is still using this.
    */
    SYSTEM_WIN2003,

    /**
     * @brief Unlikely anyone is still using this.
    */
    SYSTEM_WINVISTA,

    /**
     * @brief Windows 7.
    */
    SYSTEM_WIN7,

    /**
     * @brief Windows 8, 8.1, 10, and 11 all report they are at least 
     * Windows 8 due to some API changes. It's not really important to
     * distinguish between any of these versions anyway.
    */
    SYSTEM_WIN8_OR_LATER,

    /**
     * @brief Not possible.
    */
    SYSTEM_UNKNOWN
};

/**
 * @brief Get the current system type.
*/
IC_PUBLIC
enum system_type sys_system_type(void);

IC_PUBLIC
void sys_error(const char *err, ...);

IC_PUBLIC
void sys_sleep(int msec);

/**
 * @brief Set environment variable @p name with value @p value. If @p value is
 * NULL, then this will attempt to unset the variable.
 * 
 * @param[in] name name of environment variable
 * @param[in] value value of environment variable, or NULL to unset
 * @return true if success, false otherwise
*/
IC_PUBLIC
qbool sys_setenv(const char *name, const char *value);

/**
 * @brief Get environment variable.
 * @param[in] name name of environment variable
 * @return value of environment variable, or NULL if not set
*/
IC_PUBLIC
char *sys_getenv(const char *name);

/**
 * @brief Get @p len random bytes from the system.
 * @param[in] len number of bytes to get
 * @param[out] buf buffer to store random bytes
 * @return true if success, false otherwise
*/
IC_PUBLIC
qbool sys_random_bytes(size_t len, void *buf);

IC_PUBLIC
char *sys_default_homepath(void);

#endif /* IC_SYSTEM_H */
