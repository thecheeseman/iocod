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

#ifndef IC_PLATFORM_ENDIANNESS_H
#define IC_PLATFORM_ENDIANNESS_H

/* always include config.h */
#if defined HAVE_CONFIG_H
#include "config.h"
#endif

//
// endianness
//
extern void copy_short_swap(void *dest, void *src);
extern void copy_long_swap(void *dest, void *src);
extern short short_swap(short l);
extern int long_swap(int l);
extern float float_swap(const float *f);

/**
 * @def IC_PLATFORM_LITTLE_ENDIAN
 * @brief Defined if the system is little-endian.
 * CMake determined at compile time.
 */

/**
 * @def IC_PLATFORM_BIG_ENDIAN
 * @brief Defined if the system is big-endian.
 * CMake determined at compile time.
 */

#if defined IC_PLATFORM_LITTLE_ENDIAN && defined IC_PLATFORM_BIG_ENDIAN
#error "Platform endianness defined as both little and big"
#elif defined IC_PLATFORM_LITTLE_ENDIAN
#define copy_little_short(dest, src) memcpy(dest, src, 2);
#define copy_little_long(dest, src)  memcpy(dest, src, 4);
#define little_short(x) (x)
#define little_long(x)  (x)
#define little_float(x) (x)
#define big_short(x)    short_swap(x)
#define big_long(x)     long_swap(x)
#define big_float(x)    float_swap(&x)
#elif defined IC_PLATFORM_BIG_ENDIAN
#define copy_little_short(dest, src) copy_short_swap(dest, src)
#define copy_little_long(dest, src)  copy_long_swap(dest, src);
#define little_short(x) short_swap(x)
#define little_long(x)  long_swap(x)
#define little_float(x) float_swap(&x)
#define big_short(x)    (x)
#define big_long(x)     (x)
#define big_float(x)    (x)
#else
#error "Platform endianness not defined"
#endif

#endif /* IC_PLATFORM_ENDIANNESS_H */
