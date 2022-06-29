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

#ifndef IOCOD_CONSOLE_H
#define IOCOD_CONSOLE_H

#include "iocod.h"

#define MAX_PRINT_MSG 4096

#define IC_CONSOLE_TITLE "iocod dedicated server console [" \
    IC_VERSION_STRING "-" IC_PLATFORM_STRING "]"

/**
 * @defgroup console Console
 * @brief Console module for handling input, output, etc.
 * @{
 */

/**
 * @brief Check if the console is initialized or not.
 * @return true if the console is initialized, false otherwise.
 */
IC_PUBLIC
bool con_initialized(void);

/**
 * @brief Initialize console. Sets up input and output processing, color 
 * support (for termianls that support it), etc.
*/
IC_PUBLIC
void con_init(void);

/**
 * @brief Shutdown console.
*/
IC_PUBLIC
void con_shutdown(void);

/**
 * @brief Set the console title. Only works for supported terminals, otherwise
 * this does nothing.
 * 
 * @param[in] title new title for the console window
*/
IC_PUBLIC
void con_set_title(const char *title);

/**
 * @brief Get the latest console input.
 * @return NULL if no input, otherwise a NULL-terminated string containing the
 * last entered line
*/
IC_PUBLIC
char *con_input(void);

/**
 * @brief Print a message out to the console.
 * @param[in] msg message to print
*/
IC_PUBLIC
void con_print(const char *msg);

/** @} */

#endif /* IOCOD_CONSOLE_H */
