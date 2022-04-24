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

#ifndef IOCOD_H
#define IOCOD_H

/* always include config.h */
#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/*
================================================================================
General useful macros / utils
================================================================================
*/

/* stringify */

#define IC_STRINGIFY_EX(x)      #x
#define IC_STRINGIFY(x)         IC_STRINGIFY_EX(x)

/* concatenation */
#define IC_CONCAT_EX(a, b)      a ## b
#define IC_CONCAT(a, b)         IC_CONCAT_EX(a, b)
#define IC_CONCAT3_EX(a, b, c)  a ## b ## c
#define IC_CONCAT3(a, b, c)     IC_CONCAT3_EX(a, b, c)

/* some useful utilities from <kernel.h> */
#define ARRAY_SIZE(x)       (sizeof(x) / sizeof((x)[0]))
#define FIELD_SIZEOF(t, f)  (sizeof(((t*)0)->f))

/**
 * @def IC_VERSION_ENCODE
 * 
 * Encode separate version numbers into one large number.
 */
#define IC_VERSION_ENCODE(maj, min, rev) \
    (((maj) * 1000000) + ((min) * 1000) + (rev))

/**
 * @def IC_VERSION_DECODE_MAJOR
 * 
 * Decode the major version from the full version value.
 */
#define IC_VERSION_DECODE_MAJOR(version)        ((version) / 1000000)

/**
 * @def IC_VERSION_DECODE_MINOR
 * 
 * Decode the minor version from the full version value.
 */
#define IC_VERSION_DECODE_MINOR(version)        ((version % 1000000) / 1000)

/**
 * @def IC_VERSION_DECODE_REVISION
 * 
 * Decode the revision number from the full version value.
 */
#define IC_VERSION_DECODE_REVISION(version)     ((version) % 1000)

/**
 * @def IC_VERSION
 * 
 * The numeric representation of the version.
 */
#define IC_VERSION \
    IC_VERSION_ENCODE(IC_VERSION_MAJOR, IC_VERSION_MINOR, IC_VERSION_PATCH)

/**
 * @def IC_VERSION_STRING
 * 
 * Version string preprended with "v" and separated by ".".
 */
#define IC_VERSION_STRING \
    "v" IC_STRINGIFY(IC_VERSION_MAJOR) \
    "." IC_STRINGIFY(IC_VERSION_MINOR) \
    "." IC_STRINGIFY(IC_VERSION_PATCH)

/* separate header since it's large */
#include "ic_platform.h"

/*
================================================================================
Common types
================================================================================
*/

/**
 * @def bool 
 * 
 * These must be the same size as int for backwards compatibility reasons
 * i.e. we cannot use <stdbool.h>
 */
#ifndef _IC_DEFINED_BOOL
#define _IC_DEFINED_BOOL
#ifdef bool
#undef bool
#endif

typedef int _boolean;
#define bool _boolean

#define false 0
#define true  1
#endif

/* byte */
#ifndef _IC_DEFINED_BYTE
#define _IC_DEFINED_BYTE
typedef unsigned char byte;
#endif

/* filehandle */
#ifndef _IC_DEFINED_FILEHANDLE
#define _IC_DEFINED_FILEHANDLE
typedef size_t filehandle;
#endif

/* vectors */
#ifndef _IC_DEFINED_VECTOR
#define _IC_DEFINED_VECTOR

/* optionally we can use float for compatibility or less precision */
#ifdef IC_VECTOR_FLOAT
typedef float vec_t;
#else
typedef double vec_t;
#endif

typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];
#endif

/*
================================================================================
Assert utilities
================================================================================
*/
#include <assert.h>

/**
 * @def IC_ASSERT
 * 
 * Assert macro (essentially `assert()`). Defined for stylistic reasons.
 */
#define IC_ASSERT(expr)             assert(expr)

/**
 * @def IC_ASSERT_MSG
 * 
 * Allows defining a message along with the assert statement. Strictly, there
 * isn't much of a better way of doing this, but at least you will can give
 * useful messages instead of vague asserts.
 */
#define IC_ASSERT_MSG(expr, msg)    assert(((void) msg, expr))

/**
 * @def IC_STATIC_ASSERT
 * 
 * Compile time assertions. If we are running in C11 mode, this maps as
 * `_Static_assert`, otherwise for MSVC this is interpreted as `static_assert`.
 */
#if defined IC_PLATFORM_STD_C11 || IC_GCC_VERSION_CHECK(6, 0, 0) || \
    defined _Static_assert
#define IC_STATIC_ASSERT(expr, msg) _Static_assert(expr, msg)
#elif IC_MSVC_VERSION_CHECK(16, 0, 0)
#define IC_STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#else
#define IC_STATIC_ASSERT(expr, msg)
#endif

/*
================================================================================
Memory
================================================================================
*/

/**
 * "Safer" library replacement for `malloc`. Always allocates at least 1 byte
 * of memory, and will fatally exit if unable to allocate the memory requested.
 * 
 * @param size size in bytes to allocate
*/
IC_MALLOC 
IC_PUBLIC 
void *ic_malloc(size_t size);

/**
 * "Safer" library replacement for `free`. Always checks that the passed
 * pointer is not NULL before freeing. 
 * 
 * @param ptr memory to free
*/
IC_PUBLIC 
void ic_free(void *ptr);

/**
 * "Safer" library replacement for `calloc`. Always checks that we allocate at
 * least 1 element of size 1. Will fatally exit if unable to allocate the 
 * memory requested.
 * 
 * @param count number of elements to allocate
 * @param size size of each element
*/
IC_MALLOC 
IC_PUBLIC 
void *ic_calloc(size_t count, size_t size);

/**
 * "Safer" library replacement for `realloc`. Always checks that we have a 
 * valid pointer passed, and if the pointer is NULL, will automatically
 * allocate new memory instead of reallocating. Always makes sure to allocate
 * memory of at least 1 byte. Will fatally exit if it cannot reallocate memory.
 * 
 * @param oldptr old memory to reallocate
 * @param size new size of memory
*/
IC_PUBLIC 
void *ic_realloc(void *oldptr, size_t size);

/*
================================================================================
Common IO functions
================================================================================
*/

/**
 * Print wrapper. Allows direct output to `stderr`. If `SYS_PRINT` or `SYS_WARN`
 * macros are defined, will call either of those instead.
 * 
 * It is recommended to access this function via the macros `ic_printf()` and
 * `ic_warning()` instead of calling this directly.
 * 
 * @param warning true if this is warning, false otherwise
 * @param fmt format specifier
 * @param ... 
 * 
 * @see ic_printf
 * @see ic_warning
 */
IC_PUBLIC
IC_PRINTF_FORMAT(2, 3)
void _ic_print(bool warning, const char *fmt, ...);

/**
 * @def ic_printf
 *
 * Wrapper for `_ic_print`.
 * 
 * @see _ic_print
 */
#define ic_printf(...) _ic_print(false, __VA_ARGS__)

/**
 * @def ic_warning
 *
 * Wrapper for `_ic_print`, prepending the message passed with "WARNING".
 * 
 * @see _ic_print
 */
#define ic_warning(...) _ic_print(true, __VA_ARGS__)

/**
 * Error wrapper. Allows direct output to `stderr`, or if macro `COM_ERROR` is
 * defined, will call that instead.
 * 
 * It is recommended to access this function via the macros `ic_error()` and
 * `ic_error_fatal()` instead of calling this directly.
 *
 * @param fatal true if this is a fatal error and we should exit
 * @param filename name of file this was called from
 * @param function name of function this was called from
 * @param line line number
 * @param fmt format specifier
 * @param ...
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
 * 
 * Wrapper for _ic_error. Automatically inputs current filename, function name,
 * line number for better error messages.
 * 
 * @see _ic_error
 */
#define ic_error(...) \
    _ic_error(false, __BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * @def ic_error_fatal
 *
 * Wrapper for _ic_error. Automatically inputs current filename, function name,
 * line number for better error messages. Causes program to exit.
 *
 * @see _ic_error
 */
#define ic_error_fatal(...) \
    _ic_error(true, __BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

/*
================================================================================
String replacements
================================================================================
*/
#include <string.h>

/**
 * Duplicate a string. This is a library replacement for `strdup` if it 
 * (somehow) does not exist on the target platform. 
 * 
 * @param s string to duplicate
 */
IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strdup(const char *s);

/**
 * @def strdup
 * 
 * Redirect to `_ic_strdup()`. Only defined if the platform does not have 
 * a builtin `strdup` available.
 * 
 * This should exist on all modern POSIX/Windows systems.
 */
#if !defined HAVE_STRDUP
#define strdup _ic_strdup
#endif

/**
 * Duplicate a string of at most `n` bytes. This is a library replacement for 
 * `strndup` if it does not exist on the target platform. 
 * 
 * @param s string to duplicate
 * @param n number of bytes to duplicate
 */
IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strndup(const char *s, size_t n);

/**
 * @def strndup
 * 
 * Redirect to `_ic_strndup()`. Only defined if the platform does not have
 * a builtin `strdup` available.
 * 
 * On POSIX systems this is undefined.
 * On Windows this is defined.
 */
#if !defined HAVE_STRNDUP
#define strndup _ic_strndup
#endif

/**
 * Convert string to lowercase. This is a library replacement for `strlwr` if 
 * it does not exist on the target platform.
 * 
 * @param s string to convert to lowercase
 */
IC_PUBLIC 
IC_NON_NULL(1)
char *_ic_strlwr(char *s);

/**
 * @def strlwr
 * 
 * Redirect to `_ic_strlwr()`. Only defined if the platform does not have
 * a built in `strlwr` available.
 * 
 * POSIX does not have a builtin `strlwr`.
 * Windows has a built `strlwr`.
 */
#if !defined HAVE_STRLWR
#define strlwr _ic_strlwr
#endif

/**
 * Convert string to uppercase. This is a library replacement for `strupr` if
 * it does not exist on the target platform.
 *
 * @param s string to convert to uppercase
 */
IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strupr(char *s);

/**
 * @def strupr
 *
 * Redirect to `_ic_strupr()`. Only defined if the platform does not have
 * a built in `strupr` available.
 *
 * POSIX systems do not have a builtin `strupr`.
 * Windows has a built `strupr`.
 */
#if !defined HAVE_STRUPR
#define strupr _ic_strupr
#endif

#if defined HAVE_STRINGS_H
#include <strings.h>
#endif

/**
 * Case-insensitive string comparison. This is a library replacement for
 * `strcasecmp` in the systems that do not have this.
 * 
 * @param s1 first string to compare
 * @param s2 second string to compare
 */
IC_PUBLIC
IC_NON_NULL(1, 2)
int _ic_strcasecmp(const char *s1, const char *s2);

/**
 * Case-insensitive string comparison of at least `n` bytes. This is a library 
 * replacement for `strncasecmp` in the systems that do not have this.
 *
 * @param s1 first string to compare
 * @param s2 second string to compare
 * @param n number of bytes 
 */
IC_PUBLIC
IC_NON_NULL(1, 2)
int _ic_strncasecmp(const char *s1, const char *s2, size_t n);

/**
 * @def strcasecmp
 * 
 * Redirect to `_ic_strcasecmp()`. Only defined if the platform does not have
 * a builtin `strcasecmp`. Note on Windows this is defined as `_stricmp`.
 */

/**
 * @def strncasecmp
 *
 * Redirect to `_ic_strncasecmp()`. Only defined if the platform does not have
 * a builtin `strncasecmp`. Note on Windows this is defined as `_stricmpn`.
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

IC_PUBLIC 
IC_NON_NULL(1, 2)
void strncpyz(char *dest, const char *src, int destsize);

IC_PUBLIC
IC_NON_NULL(1, 3)
void strncatz(char *dest, int size, const char *src);

/**
 * @brief snprintf which always ensures there is a trailing '\0'.
 *
 * This function will always return either -1 if an error occured,
 * or a non-negative number if no error. It is possible that the
 * final string is truncated due to a smaller size than the format
 * string, in which case the returned value is the number of bytes
 * that _would_ have been written. If return value is non-negative
 * and less than size, success
 *
 * @param dest destination char array to write to
 * @param size size of array, should always be sizeof(arr)
 * @param fmt printf format specifier
 * @return -1 if error, non-negative number otherwise
*/
IC_PUBLIC 
IC_PRINTF_FORMAT(3, 4) 
IC_NON_NULL(1) 
int snprintfz(char *dest, size_t size, const char *fmt, ...);

/*
================================================================================
SAL stuff
================================================================================
*/

/* from nvim source, for unused variables */
#ifndef UNUSED
#if defined HAVE_ATTRIBUTE_UNUSED 
#define UNUSED __attribute__((unused))
#else 
#if defined __has_attribute
#if __has_attribute(unused)
#define UNUSED __attribute__((unused))
#endif /* __has_attribute(unused) */
#endif /* defined __has_attribute */
#endif /* HAVE_ATTRIBUTE_UNUSED */
#ifndef UNUSED
#define UNUSED
#endif /* UNUSED */
#endif

/* based on above, but for deprecated features */
#ifndef DEPRECATED
#if defined HAVE_ATTRIBUTE_DEPRECATED
#define DEPRECATED __attribute__((deprecated))
#else
#if defined __has_attribute
#if __has_attribute(deprecated)
#define DEPRECATED __attribute__((deprecated))
#endif /* __has_attribute(deprecated) */
#endif /* defined __has_attribute */
#endif /* HAVE_ATTRIBUTE_DEPRECATED */
#ifndef DEPRECATED
#define DEPRECATED
#endif /* DEPRECATED */
#endif

/* unused parameters */
#define UNUSED_PARAM(x) (void)(x)
#define IN
#define OUT
#define OPTIONAL

/* utilities for function marking */
#define INCOMPLETE
#define NOT_WORKING

#endif /* IOCOD_H */
