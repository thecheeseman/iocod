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

#ifndef IC_SAL_H
#define IC_SAL_H

/**
 * @addtogroup platform
 *
 * @{
 */

/*
 * SAL stuff
 * ignored in Doxygen
 */
#ifdef IC_PLATFORM_MSVC
#include <sal.h>
#endif

#ifndef _In_
#define _In_
#endif

#ifndef _In_z_
#define _In_z_
#endif

#ifndef _Inout_
#define _Inout_
#endif

#ifndef _In_opt_
#define _In_opt_
#endif

#ifndef _In_opt_z_
#define _In_opt_z_
#endif

#ifndef _Out_
#define _Out_
#endif

#ifndef _Out_opt_
#define _Out_opt_
#endif

#ifndef _Out_writes_
#define _Out_writes_(size)
#endif

#ifndef _Out_writes_z_
#define _Out_writes_z_(size)
#endif

/**
 * @def IC_PRINTF_FORMAT
 * @brief This attribute tells the compiler to check the arguments for
 consistency with `printf`-style formatted strings.
 *
 * @param idx   starting index of the format specifier
 * @param first starting index of the varargs
 * @param var   format variable (for MSVC)
 *
 * For example:
 * @code
 *  IC_PRINTF_FORMAT(2, 3, fmt)
 *  void print_error(int code, const char *fmt, ...);
 * @endcode
 *
 * @note On GNUC compilers, this is defined as 
 * `__attribute__((format(printf, idx, first)))`.
 * @note On MSVC this is defined as with the SAL annotation 
 * `_At_(var, _Printf_format_string_)`
 *
 * @since 0.110.0
 */
#ifdef IC_PLATFORM_MSVC
#define IC_PRINTF_FORMAT(idx, first, var) _At_(var, _Printf_format_string_)
#elif defined __MINGW32__ && IC_GCC_HAS_ATTRIBUTE(format, 4, 4, 0) && \
    !defined __USE_MINGW_ANSI_STDIO
#define IC_PRINTF_FORMAT(idx, first, var) __attribute__((format(ms_printf, idx, first)))
#elif defined __MINGW32__ && IC_GCC_HAS_ATTRIBUTE(format, 4, 4, 0) && \
    defined __USE_MINGW_ANSI_STDIO
#define IC_PRINTF_FORMAT(idx, first, var) __attribute__((format(gnu_printf, idx, first)))
#elif IC_HAS_ATTRIBUTE(format) || IC_GCC_VERSION_CHECK(3, 1, 0)
#define IC_PRINTF_FORMAT(idx, first, var) __attribute__((format(printf, idx, first)))
#else
#define IC_PRINTF_FORMAT(idx, first, var)
#endif

/**
 * @def IC_WARN_UNUSED_RESULT
 * @brief This attribute allows compatible compilers to generate a warning
 * if the returned result of the function is ignored.
 *
 * @note On GNUC this is defined as `__attribute__((__warn_unused_result__))`.
 * @note On MSVC this is defined as the SAL annotation `_Check_return_`.
 *
 * @since 0.110.0
 */
#ifdef _Check_return_
#define IC_WARN_UNUSED_RESULT _Check_return_
#elif IC_HAS_ATTRIBUTE(warn_unused_result) || IC_GCC_VERSION_CHECK(3, 4, 0)
#define IC_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#else
#define IC_WARN_UNUSED_RESULT
#endif

/**
 * @def IC_RETURNS_NON_NULL
 * @brief This attribute allows compilers to better optimise functions knowing
 * that they will never return NULL.
 *
 * @note On GNUC this is defined as `__attribute__((__returns_nonnull__)`.
 * @note On MSVC this is defined with the SAL annotation `_Ret_notnull_`.
 *
 * @since 0.110.0
 */
#ifdef _Ret_notnull_
#define IC_RETURNS_NON_NULL _Ret_notnull_
#elif IC_HAS_ATTRIBUTE(returns_nonnull) || IC_GCC_VERSION_CHECK(4, 9, 0)  
#define IC_RETURNS_NON_NULL __attribute__((__returns_nonnull__))
#else
#define IC_RETURNS_NON_NULL
#endif

/**
 * @def IC_RETURNS_STRING
 * @brief Microsoft-specific annotation to indicate that the function returns a
 * string.
 * @since 0.205.0
 */
#ifdef _Ret_z_
#define IC_RETURNS_STRING _Ret_z_
#else
#define IC_RETURNS_STRING
#endif

/**
 * @def UNUSED_PARAM
 * @brief Mark a parameter as unused in function body. Effectively silences
 * compiler warnings about unused parameters.
 */
#define UNUSED_PARAM(x) (void)(x)

/* utilities for function marking */
#define INCOMPLETE
#define NOT_WORKING

/** @} */

#endif /* IC_SAL_H */
