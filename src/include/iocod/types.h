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

#ifndef IOCOD_TYPES_H
#define IOCOD_TYPES_H

#include "iocod/platform.h"
#include <inttypes.h>
#include <sys/types.h>

/**
 * @defgroup common_types Common Types
 * @brief Various common types used throughout the project.
 * @{
 */

/**
 * @brief Kernel shorthands
 * @{
 */
typedef int8_t      s8;
typedef uint8_t     u8;
typedef int16_t     s16;
typedef uint16_t    u16;
typedef int32_t     s32;
typedef uint32_t    u32;
typedef int64_t     s64;
typedef uint64_t    u64;
/** @} */

#ifdef IC_PLATFORM_WINDOWS
#if IC_PLATFORM_64BIT
typedef long long ssize_t;
#else
typedef long ssize_t;
#endif
#endif

/**
 * @brief boolean type
 *
 * `bool` must always be the same size as `int` for compatibility (structure
 * field sizes must match), therefore we _cannot_ use `<stdbool.h>`
*/
typedef int qbool;

#ifndef __cplusplus
#define false 0
#define true  1
#endif

/**
 * @brief Shorthand for `unsigned char`.
*/
typedef unsigned char byte;

/**
 * @typedef size_t filehandle
 * @brief File handle reference.
 *
 * Strictly speaking, since we probably are never going to handle more than
 * a couple thousand files at a time, `int` sound be fine here (as it was
 * in the original source). In the future this may mean a hash of a given file,
 * rather than just meaning 'nth open file' as it currently does.
*/
typedef int filehandle;

/**
 * @typedef float vec_t
 * @brief Basic vector unit for math.
 */
typedef float vec_t;

/**
 * @brief Array of vectors.
 * @{
*/
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];
/** @} */

/** @} */

#endif /* IOCOD_TYPES_H */
