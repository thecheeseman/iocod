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

#ifndef IC_STRING_H
#define IC_STRING_H

#include <string.h>

#if !defined HAVE_STRDUP 
IC_PUBLIC 
char *strdup(const char *s);
#endif

#if !defined HAVE_STRNDUP
IC_PUBLIC 
char *strndup(const char *s, size_t n);
#endif

#if !defined HAVE_STRLWR
IC_PUBLIC 
char *strlwr(char *s1);
#endif

#if !defined HAVE_STRUPR
IC_PUBLIC 
char *strupr(char *s1);
#endif

#if defined HAVE_STRINGS_H
#include <strings.h>
#endif

#if defined IC_PLATFORM_WINDOWS
#define HAVE_STRCASECMP  1
#define HAVE_STRNCASECMP 1

#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#else
#if !defined HAVE_STRCASECMP
IC_PUBLIC 
int strcasecmp(const char *s1, const char *s2);
#endif

#if !defined HAVE_STRNCASECMP
IC_PUBLIC 
int strncasecmp(const char *s1, const char *s2, size_t n);
#endif
#endif /* IC_PLATFORM_WINDOWS */

IC_PUBLIC 
void strncpyz(char *dest, const char *src, int destsize);

IC_PUBLIC 
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
IC_PUBLIC IC_PRINTF_FORMAT(3, 4) int snprintfz(char *dest, 
                                               size_t size, 
                                               const char *fmt, 
                                               ...);

#endif /* IC_STRING_H */
