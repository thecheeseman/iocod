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

#ifndef IC_TYPES_H
#define IC_TYPES_H

/*
 * Boolean
 * These must be the same size as int for backwards compatibility reasons
 * i.e. we cannot use <stdbool.h> 
 */
#ifndef IC_DEFINED_BOOL
#define IC_DEFINED_BOOL

#ifdef bool
#undef bool
#endif

typedef int _boolean;
#define bool _boolean

#define false 0
#define true  1
#endif /* IC_DEFINED_BOOL */

/*
 * Byte
 */
#ifndef IC_DEFINED_BYTE
#define IC_DEFINED_BYTE

typedef unsigned char byte;
#endif /* IC_DEFINED_BYTE */

/*
 * Filehandle
 */
#ifndef IC_DEFINED_FILEHANDLE
#define IC_DEFINED_FILEHANDLE

typedef size_t filehandle;
#endif /* IC_DEFINED_FILEHANDLE */

/*
 * NULL
 * In the event some system doesn't define this
 */
#ifndef IC_DEFINED_NULL
#define IC_DEFINED_NULL

#ifndef NULL
#define NULL ((void *) 0)
#endif
#endif /* IC_DEFINED_NULL */

/*
 * Vectors
 */
#ifndef IC_DEFINED_VECTOR
#define IC_DEFINED_VECTOR

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];
typedef vec_t vec5_t[5];

#endif /* IC_DEFINED_VECTOR */

#endif /* IC_TYPES_H */
