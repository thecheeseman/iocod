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

#ifndef IC_SYSTEM_H
#define IC_SYSTEM_H

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

#endif /* IC_SYSTEM_H */
