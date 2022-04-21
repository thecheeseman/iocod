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

#ifndef IC_SAL_H
#define IC_SAL_H

/*
 * From nvim, for unused variables
 */
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

/*
 * Based on above, but for deprecated features
 */
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

/*
 * Unused parameters
 */
#define UNUSED_PARAM(x) (void)(x)
#define IN
#define OUT
#define OPTIONAL

/*
 * Utilities for function marking
 */
#define INCOMPLETE
#define NOT_WORKING

#endif /* IC_SAL_H */
