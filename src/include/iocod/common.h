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

extern struct cvar *com_dedicated;
extern struct cvar *com_sv_running;

IC_PUBLIC
void com_init(void);

IC_PUBLIC
void com_shutdown(void);

IC_PUBLIC
void com_frame(void);

IC_PUBLIC
bool com_filter(char *filter, char *name, bool casesensitive);

IC_PUBLIC
char *com_string_contains(char *str1, char *str2, bool casesensitive);

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
IC_PRINTF_FORMAT(1, 2)
void ic_printf(const char *fmt, ...);

#define ic_warning(...) ic_printf(__VA_ARGS__)

#endif /* IC_COMMON_H */
