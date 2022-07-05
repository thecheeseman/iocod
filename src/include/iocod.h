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

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>

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

#include "iocod/version.h"

/*
================================================================================
Platform utilities
================================================================================
*/

#include "iocod/platform.h"
#include "iocod/platform_endianness.h"

/*
================================================================================
Common types
================================================================================
*/

// must be included before any other shared includes
#include "iocod/types.h" 

/*
================================================================================
iocod includes
================================================================================
*/

#include "iocod/assert.h"
#include "iocod/client.h"
#include "iocod/commands.h"
#include "iocod/common.h"
#include "iocod/conf.h"
#include "iocod/console.h"
#include "iocod/cvar.h"
#include "iocod/events.h"
#include "iocod/log.h"
#include "iocod/memory.h"
#include "iocod/network.h"
#include "iocod/parse.h"
#include "iocod/server.h"
#include "iocod/shared.h"
#include "iocod/string.h"
#include "iocod/system.h"

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
