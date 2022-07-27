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

#ifndef IC_UTILS_H
#define IC_UTILS_H

/*
================================================================================
General useful macros / utils
================================================================================
*/

/* gettext style strings for later */
#define _(str) (str)
#define N_(str) str

/**
 * @def ARRAY_SIZE
 * @brief Determine size of a given array at compile-time. From `<kernel.h>`
 */
#define ARRAY_SIZE(x)       (sizeof(x) / sizeof((x)[0]))

/**
 * @def FIELD_SIZEOF
 * @brief Determine the size of a given field in a given struct.
 *
 * From `<kernel.h>`
 *
 * Example:
 * @code
 * struct coolstruct {
 *     size_t field1;
 * };
 *
 * int size = FIELD_SIZEOF(struct coolstruct, field1); // 4 on 32-bit, 8 on 64-bit
 * @endcode
 */
#define FIELD_SIZEOF(t, f)  (sizeof(((t*)0)->f))

/**
 * @defgroup stringmacros String Macros
 * @brief Useful string macros for stringifying or concatenation.
 * @{
 */

/**
 * @def IC_STRINGIFY
 * @brief Stringify.
 */
#define IC_STRINGIFY_EX(x)      #x
#define IC_STRINGIFY(x)         IC_STRINGIFY_EX(x)

/**
 * @def IC_CONCAT
 * @brief Concatenate two things.
 */
#define IC_CONCAT_EX(a, b)      a ## b
#define IC_CONCAT(a, b)         IC_CONCAT_EX(a, b)

/**
 * @def IC_CONCAT3
 * @brief Concatenate three things.
 */
#define IC_CONCAT3_EX(a, b, c)  a ## b ## c
#define IC_CONCAT3(a, b, c)     IC_CONCAT3_EX(a, b, c)
/** @} */

#endif /* IC_UTILS_H */
