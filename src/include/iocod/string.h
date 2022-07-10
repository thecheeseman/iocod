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

#ifndef IC_STRING_H
#define IC_STRING_H

#include "iocod/platform.h"
#include "iocod/types.h"
#include <string.h>

/**
 * @defgroup string_lib String Library
 * @brief Library replacements or additions for handling strings.
 * @{
 */

/**
 * @brief Duplicate a string.
 *
 * This is a library replacement for `strdup` if it (somehow) does not
 * already exist on the target platform.
 *
 * @param[in] s string to duplicate
 */
IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strdup(const char *s);

/**
 * @def strdup
 * @brief Redirect to `_ic_strdup()`.
 *
 * This is only defined if the platform does not have a builtin `strdup`
 * available -- i.e. this should exist on all modern POSIX/Windows systems.
 */
#if !defined HAVE_STRDUP
#define strdup _ic_strdup
#endif

/**
 * @brief Duplicate a string of at most `n` bytes.
 *
 * This is a library replacement for `strndup` if it does not exist on
 * the target platform.
 *
 * @param[in] s string to duplicate
 * @param[in] n number of bytes to duplicate
 */
IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strndup(const char *s, size_t n);

/**
 * @def strndup
 * @brief Redirect to `_ic_strndup()`.
 *
 * Only defined if the platform does not have a builtin `strdup` available.
 *
 * @note On POSIX systems this is not defined.
 * @note On Windows this is defined.
 */
#if !defined HAVE_STRNDUP
#define strndup _ic_strndup
#endif

/**
 * @brief Convert string to lowercase.
 *
 * This is a library replacement for `strlwr` if it does not exist on the
 * target platform, e.g. POSIX.
 *
 * @param[in,out] s string to convert to lowercase
 */
IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strlwr(char *s);

/**
 * @def strlwr
 * @brief Redirect to `_ic_strlwr()`.
 *
 * Only defined if the platform does not have a built in `strlwr` available.
 *
 * @note POSIX does not have a builtin `strlwr`.
 * @note Windows has a built `strlwr`.
 */
#if !defined HAVE_STRLWR
#define strlwr _ic_strlwr
#endif

/**
 * @brief Convert string to uppercase.
 *
 * This is a library replacement for `strupr` if it does not exist on
 * the target platform.
 *
 * @param[in,out] s string to convert to uppercase
 */
IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strupr(char *s);

/**
 * @def strupr
 * @brief Redirect to `_ic_strupr()`.

 * Only defined if the platform does not have a built in `strupr` available.
 *
 * @note POSIX systems do not have a builtin `strupr`.
 * @note Windows has a built `strupr`.
 */
#if !defined HAVE_STRUPR
#define strupr _ic_strupr
#endif

#if defined HAVE_STRINGS_H
#include <strings.h>
#endif

/**
 * @brief Case-insensitive string comparison.
 *
 * This is a library replacement for `strcasecmp` in the systems
 * that do not have this.
 *
 * @param[in] s1 first string to compare
 * @param[in] s2 second string to compare
 */
IC_PUBLIC
IC_NON_NULL(1, 2)
int _ic_strcasecmp(const char *s1, const char *s2);

/**
 * @brief Case-insensitive string comparison of at least `n` bytes.
 *
 * This is a library  replacement for `strncasecmp` in the systems
 * that do not have this.
 *
 * @param[in] s1 first string to compare
 * @param[in] s2 second string to compare
 * @param[in] n  number of bytes
 */
IC_PUBLIC
IC_NON_NULL(1, 2)
int _ic_strncasecmp(const char *s1, const char *s2, size_t n);

/**
 * @def strcasecmp
 * @brief Redirect to `_ic_strcasecmp()`.
 *
 * Only defined if the platform does not have a builtin `strcasecmp`.
 * @note On Windows this is defined as `_stricmp`.
 */

/**
 * @def strncasecmp
 * @brief Redirect to `_ic_strncasecmp()`.
 *
 * Only defined if the platform does not have a builtin `strncasecmp`.
 *
 * @note On Windows this is defined as `_stricmpn`.
 */
#if defined IC_PLATFORM_WINDOWS
#define HAVE_STRCASECMP  1
#define HAVE_STRNCASECMP 1

#define strcasecmp  _stricmp
#define strncasecmp _strnicmp
#else
#if !defined HAVE_STRCASECMP 
#define strcasecmp _ic_strcasecmp
#endif

#if !defined HAVE_STRNCASECMP
#define strncasecmp _ic_strncasecmp
#endif
#endif /* IC_PLATFORM_WINDOWS */

/**
 * @brief `strncpy` replacement which always ensures a trailing '\0'.
 *
 * @param[out] dest     destination to copy the string
 * @param[in]  src      source string to copy
 * @param[in]  destsize size of the destination
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
void strncpyz(char *dest, const char *src, size_t destsize);

/**
 * @brief `strcat` replacement which always ensures there is a trailing '\0'.
 *
 * @param[out] dest destination string
 * @param[in]  size size of destination string
 * @param[in]  src  source string to copy
*/
IC_PUBLIC
IC_NON_NULL(1, 3)
void strncatz(char *dest, size_t size, const char *src);

/**
 * @brief `snprintf` replacement which always ensures there is a trailing '\0'.
 *
 * This function will always return either -1 if an error occured,
 * or a non-negative number if no error. It is possible that the
 * final string is truncated due to a smaller size than the format
 * string, in which case the returned value is the number of bytes
 * that _would_ have been written. If return value is non-negative
 * and less than size, success
 *
 * @param[out] dest destination char array to write to
 * @param[in]  size size of array, should always be sizeof(arr)
 * @param[in]  fmt  printf format specifier
 * @return -1 if error, non-negative number otherwise
*/
IC_PUBLIC
IC_PRINTF_FORMAT(3, 4)
IC_NON_NULL(1)
ssize_t snprintfz(char *dest, size_t size, const char *fmt, ...);

IC_PUBLIC
char *copy_string(const char *in);

/** @} */

#endif /* IC_STRING_H */
