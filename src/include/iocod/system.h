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
bool sys_library_load(const char *path, void **handle);

/**
 * @brief Close a system library.
 * @param[in] handle loaded library module
 * @return true if success, false otherwise
*/
IC_PUBLIC
bool sys_library_close(void *handle);

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
bool sys_library_load_symbol(void *handle, const char *fn, void **symbol);

/**
 * @brief Get current milliseconds since application began.
 * @return current milliseconds
*/
IC_PUBLIC
int32_t sys_milliseconds(void);

IC_PUBLIC
IC_NO_RETURN
void sys_exit(int code);

IC_PUBLIC
void sys_quit(void);

IC_PUBLIC
void sys_setup_signal_handler(void);

IC_PUBLIC
void sys_set_floatenv(void);

#endif /* IC_SYSTEM_H */
