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

/**
 * @file   iocod.h
 * @author thecheeseman
 * @date   2022-04-26
 * 
 * Global header for iocod project. Contains useful macros for platform and
 * compiler identification, features, etc., custom string library, memory
 * allocation functions.
 * 
 * Automatically includes `<stdarg.h>`, `<stddef.h>`, `<stdint.h>`, and 
 * `<stdio.h>`.
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

/**
 * @defgroup version_macros Version Macros
 * @brief Macros for dealing with version data.
 * @{ 
 */

/**
 * @def IC_VERSION_ENCODE
 * @brief Encode separate version numbers into one large number.
 */
#define IC_VERSION_ENCODE(maj, min, rev) \
    (((maj) * 1000000) + ((min) * 1000) + (rev))

/**
 * @def IC_VERSION_DECODE_MAJOR
 * @brief Decode the major version from the full version value.
 */
#define IC_VERSION_DECODE_MAJOR(version)        ((version) / 1000000)

/**
 * @def IC_VERSION_DECODE_MINOR
 * @brief Decode the minor version from the full version value.
 */
#define IC_VERSION_DECODE_MINOR(version)        ((version % 1000000) / 1000)

/**
 * @def IC_VERSION_DECODE_REVISION
 * @brief Decode the revision number from the full version value.
 */
#define IC_VERSION_DECODE_REVISION(version)     ((version) % 1000)

/**
 * @def IC_VERSION_MAJOR
 * @brief Major version number. Automatically filled out by CMake.
 */
#ifndef IC_VERSION_MAJOR
#define IC_VERSION_MAJOR 0
#endif

/**
 * @def IC_VERSION_MINOR
 * @brief Minor version number. Automatically filled out by CMake.
 */
#ifndef IC_VERSION_MINOR
#define IC_VERSION_MINOR 0
#endif

/**
 * @def IC_VERSION_PATCH
 * @brief Patch version number. Automatically filled out by CMake.
 */
#ifndef IC_VERSION_PATCH
#define IC_VERSION_PATCH 0
#endif

/**
 * @def IC_VERSION
 * @brief The numeric representation of the version.
 */
#define IC_VERSION \
    IC_VERSION_ENCODE(IC_VERSION_MAJOR, IC_VERSION_MINOR, IC_VERSION_PATCH)

/**
 * @def IC_VERSION_STRING
 * @brief Version string preprended with "v" and separated by ".".
 */
#define IC_VERSION_STRING \
    "v" IC_STRINGIFY(IC_VERSION_MAJOR) \
    "." IC_STRINGIFY(IC_VERSION_MINOR) \
    "." IC_STRINGIFY(IC_VERSION_PATCH)

/** @} */

/* separate header since it's large */
#include "ic_platform.h"

/*
================================================================================
Common types
================================================================================
*/

/**
 * @defgroup common_types Common Types
 * @brief Various common types used throughout the project.
 * @{ 
 */
#ifndef _IC_DEFINED_BOOL
#define _IC_DEFINED_BOOL
#ifdef bool
#undef bool
#endif

/**
 * @brief boolean type
 * 
 * `bool` must always be the same size as `int` for compatibility (structure
 * field sizes must match), therefore we _cannot_ use `<stdbool.h>`
 * @{
*/
typedef int _boolean;
#define bool _boolean
/** @} */

#define false 0
#define true  1
#endif

#ifndef _IC_DEFINED_BYTE
#define _IC_DEFINED_BYTE

/**
 * @brief Shorthand for `unsigned char`.
*/
typedef unsigned char byte;
#endif

#ifndef _IC_DEFINED_FILEHANDLE
#define _IC_DEFINED_FILEHANDLE

/**
 * @typedef size_t filehandle
 * @brief File handle reference.
 * 
 * Strictly speaking, since we probably are never going to handle more than
 * a couple thousand files at a time, `int` sound be fine here (as it was
 * in the original source). In the future this may mean a hash of a given file,
 * rather than just meaning 'nth open file' as it currently does.
*/
typedef size_t filehandle;
#endif

#ifndef _IC_DEFINED_VECTOR
#define _IC_DEFINED_VECTOR

/**
 * @typedef double vec_t
 * @brief Basic vector unit for math.
 *
 * If `IC_VECTOR_FLOAT` is defined, this will instead be `float`. 
 */
#ifdef IC_VECTOR_FLOAT
typedef float vec_t;
#else
typedef double vec_t;
#endif

/**
 * @brief Array of vectors.
 * @{
*/
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];
/** @} */
#endif

/** @} */

/*
================================================================================
Assert utilities
================================================================================
*/
#include <assert.h>

/**
 * @defgroup assert_utilities Assert Utilities 
 * @brief Assert utilities for testing code.
 * @{
 */

/**
 * @def IC_ASSERT
 * @brief Assert macro (essentially `assert()`). Defined for stylistic reasons.
 */
#define IC_ASSERT(expr)             assert(expr)

/**
 * @def IC_ASSERT_MSG
 * @brief Allows defining a message along with the assert statement. 
 * 
 * Strictly, there isn't much of a better way of doing this, but at least 
 * you can give useful messages instead of vague asserts.
 */
#define IC_ASSERT_MSG(expr, msg)    assert(((void) msg, expr))

/**
 * @def IC_STATIC_ASSERT
 * @brief Compile time assertions. 
 * 
 * @note In C11, this is defined as `_Static_assert`.
 * @note On MSVC this is defined as `static_assert`.
 */
#if defined IC_PLATFORM_STD_C11 || IC_GCC_VERSION_CHECK(6, 0, 0) || \
    defined _Static_assert
#define IC_STATIC_ASSERT(expr, msg) _Static_assert(expr, msg)
#elif IC_MSVC_VERSION_CHECK(16, 0, 0)
#define IC_STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#else
#define IC_STATIC_ASSERT(expr, msg)
#endif

/** @} */

/*
================================================================================
Memory
================================================================================
*/

/**
 * @defgroup memory Memory
 * @brief Library replacement memory functions.
 * @{
 */

/**
 * @brief "Safer" library replacement for `malloc`. 
 
 * Always allocates at least 1 byte of memory, and will fatally exit if 
 * unable to allocate the memory requested.
 * 
 * @param[in] size size in bytes to allocate
*/
IC_MALLOC 
IC_PUBLIC 
void *ic_malloc(size_t size);

/**
 * @brief "Safer" library replacement for `free`. 
 * 
 * Always checks that the passed pointer is not NULL before freeing. 
 * 
 * @param[in] ptr memory to free
*/
IC_PUBLIC 
void ic_free(void *ptr);

/**
 * @brief "Safer" library replacement for `calloc`. 
 *
 * Always checks that we allocate at least 1 element of size 1. Will fatally 
 * exit if unable to allocate the memory requested.
 * 
 * @param[in] count number of elements to allocate
 * @param[in] size size of each element
*/
IC_MALLOC 
IC_PUBLIC 
void *ic_calloc(size_t count, size_t size);

/**
 * @brief "Safer" library replacement for `realloc`. 
 *
 * Always checks that we have a valid pointer passed, and if the pointer 
 * is NULL, will automatically allocate new memory instead of reallocating. 
 * Always makes sure to allocate memory of at least 1 byte. Will fatally exit 
 * if it cannot reallocate memory.
 * 
 * @param[in] oldptr old memory to reallocate
 * @param[in] size new size of memory
*/
IC_PUBLIC 
void *ic_realloc(void *oldptr, size_t size);

/** @} */

/*
================================================================================
Common IO functions
================================================================================
*/

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
IC_PRINTF_FORMAT(2, 3)
void _ic_print(bool warning, const char *fmt, ...);

/**
 * @def ic_printf
 * @brief Wrapper for `_ic_print`.
 * @see _ic_print
 */
#define ic_printf(...) _ic_print(false, __VA_ARGS__)

/**
 * @def ic_warning
 * @brief Wrapper for `_ic_print`, prepending the message passed with "WARNING".
 * @see _ic_print
 */
#define ic_warning(...) _ic_print(true, __VA_ARGS__)

/**
 * @brief Error wrapper that allows direct output to `stderr`.
 * 
 * If `COM_ERROR` is defined, will call that instead.
 * 
 * @note It is recommended to access this function via the macros 
 * `ic_error()` and `ic_error_fatal()` instead of calling this directly.
 *
 * @param[in] fatal    true if this is a fatal error and we should exit
 * @param[in] filename name of file this was called from
 * @param[in] function name of function this was called from
 * @param[in] line     line number
 * @param[in] fmt      format specifier
 * @param[in] ...
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
 * @brief Wrapper for _ic_error. 
 *
 * Automatically inputs current filename, function name, line number for 
 * better error messages.
 * 
 * @see _ic_error
 */
#define ic_error(...) \
    _ic_error(false, __BASE_FILE__, __func__, __LINE__, __VA_ARGS__)

/**
 * @def ic_error_fatal
 * @brief Wrapper for _ic_error. 
 *
 * Automatically inputs current filename, function name, line number for 
 * better error messages. Causes program to exit.
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
size_t snprintfz(char *dest, size_t size, const char *fmt, ...);

/** @} */

/*
================================================================================
Logging
================================================================================
*/

#include "ic_log.h"

/*
================================================================================
Cvars
================================================================================
*/

#include "ic_cvar.h"

/*
================================================================================
Shared 
================================================================================
*/

#define MAX_INFO_STRING 1024
#define INFO_STRING_BIG 8192

/**
 * @brief Take a given path and append an extension (if one doesn't already
 * exist).
 *
 * If the given path string already has an extension (regardless if it is
 * the same as `ext`, will not append anything).
 *
 * @param[in,out] path string path
 * @param[in]     size max size of string
 * @param[in]     ext  extension to append
 */
IC_PUBLIC
IC_NON_NULL(1, 3)
void ic_default_extension(char *path, size_t size, const char *ext);

/**
 * @brief Remove a key/value pair from an info string.
 *
 * @param[in,out] s   info string to modify
 * @param[in]     key key to remove
 * @return true if successful, false otherwise
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
bool if_remove_key(char *s, const char *key);

/**
 * @brief Remove a key/value pair from a big info string.
 * 
 * @param[in,out] s   info string to modify
 * @param[in]     key key to remove
 * @return true if successful, false otherwise
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
bool ifbig_remove_key(char *s, const char *key);

/**
 * @brief Change or add a key/value pair to an info string.
 *
 * @param[in,out] s     info string to modify
 * @param[in]     key   key to add
 * @param[in]     value value of key
 * @return true if successful, false otherwise
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
bool if_set_value_for_key(char *s, const char *key, const char *value);

/**
 * @brief Change or add a key/value pair to a big info string.
 * 
 * @param[in,out] s     info string to modify
 * @param[in]     key   key to add
 * @param[in]     value value of key
 * @return true if successful, false otherwise
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
bool ifbig_set_value_for_key(char *s, const char *key, const char *value);

IC_PUBLIC
/**
 * @brief Create temporary vectors. 
 * 
 * @param[in] x 
 * @param[in] y
 * @param[in] z
 * @return temporary vector of (x, y, z)
*/
vec_t *tv(vec_t x, vec_t y, vec_t z);

/**
 * @brief Formatted string concatenation.
 * 
 * @param[in] fmt string format
 * @return formatted string
 */
IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
char *va(const char *fmt, ...);

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

/**
 * @def UNUSED_PARAM
 * @brief Mark a parameter as unused in function body. Effectively silences
 * compiler warnings about unused parameters.
 */
#define UNUSED_PARAM(x) (void)(x)

#define IN
#define OUT
#define OPTIONAL

/* utilities for function marking */
#define INCOMPLETE
#define NOT_WORKING

#endif /* IOCOD_H */
