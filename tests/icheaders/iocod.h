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

/* temp */
#define ic_warn_if(expr, warning)
#define ic_warning(warning)
#define ic_error(err)
#define ic_error_fatal(err)

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

/*
================================================================================
Version + platform utilities from hedley https://github.com/nemequ/hedley
================================================================================
*/
#define IC_VERSION_ENCODE(maj, min, rev) \
    (((maj) * 1000000) + ((min) * 1000) + (rev))
#define IC_VERSION_DECODE_MAJOR(version)        ((version) / 1000000)
#define IC_VERSION_DECODE_MINOR(version)        ((version % 1000000) / 1000)
#define IC_VERSION_DECODE_REVISION(version)     ((version) % 1000)

#define IC_VERSION \
    IC_VERSION_ENCODE(IC_VERSION_MAJOR, IC_VERSION_MINOR, IC_VERSION_PATCH)

#define IC_VERSION_STRING \
    "v" IC_STRINGIFY(IC_VERSION_MAJOR) \
    "." IC_STRINGIFY(IC_VERSION_MINOR) \
    "." IC_STRINGIFY(IC_VERSION_PATCH)

/* separate header since it's large */
#include "ic_platform.h"

/*
================================================================================
String replacements
================================================================================
*/
#include <string.h>

#if !defined HAVE_STRDUP 
IC_PUBLIC char *strdup(const char *s);
#endif

/* some systems do not have this (e.g. windows) */
#if !defined HAVE_STRNDUP
IC_PUBLIC char *strndup(const char *s, size_t n);
#endif

#if !defined HAVE_STRLWR
IC_PUBLIC char *strlwr(char *s1);
#endif

#if !defined HAVE_STRUPR
IC_PUBLIC char *strupr(char *s1);
#endif

#if defined HAVE_STRINGS_H
#include <strings.h>
#endif

#if defined IC_PLATFORM_WINDOWS
#define HAVE_STRCASECMP  1
#define HAVE_STRNCASECMP 1

#define strcasecmp  _stricmp
#define strncasecmp _strnicmp
#else
#if !defined HAVE_STRCASECMP 
IC_PUBLIC int strcasecmp(const char *s1, const char *s2);
#endif

#if !defined HAVE_STRNCASECMP
IC_PUBLIC int strncasecmp(const char *s1, const char *s2, size_t n);
#endif
#endif /* IC_PLATFORM_WINDOWS */

IC_PUBLIC void strncpyz(char *dest, const char *src, int destsize);

IC_PUBLIC void strncatz(char *dest, int size, const char *src);

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
Assert utilities
================================================================================
*/
#include <assert.h>

#define IC_ASSERT(expr)             assert(expr)
#define IC_ASSERT_MSG(expr, msg)    assert(((void) msg, expr))

/* Static assert */
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
Common types
================================================================================
*/

/*
 * Boolean
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
Memory
================================================================================
*/

/**
 * @brief
 * @param size
 * @return
*/
IC_MALLOC IC_PUBLIC void *ic_malloc(size_t size);

/**
 * @brief
 * @param ptr
*/
IC_PUBLIC void ic_free(void *ptr);

/**
 * @brief
 * @param count
 * @param size
 * @return
*/
IC_MALLOC IC_PUBLIC void *ic_calloc(size_t count, size_t size);

/**
 * @brief
 * @param oldptr
 * @param size
 * @return
*/
IC_PUBLIC void *ic_realloc(void *oldptr, size_t size);

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
