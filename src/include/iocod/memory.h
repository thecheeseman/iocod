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

#ifndef IOCOD_MEMORY_H
#define IOCOD_MEMORY_H

#include "iocod/platform.h"
#include "iocod/types.h"

/**
 * @defgroup memory Memory
 * @brief Memory module. Contains functions for alloc/dealloc heap and hunk
 * memory.
 * 
 * @{
 */

IC_PUBLIC
void hunk_init(void);

IC_PUBLIC
void hunk_shutdown(void);

IC_PUBLIC
void hunk_clear(void);

IC_PUBLIC
void hunk_clear_to_start(void);

IC_PUBLIC
void *hunk_alloc_align(size_t size, size_t align);

IC_PUBLIC
void *hunk_alloc(size_t size);

IC_PUBLIC
void *hunk_alloc_low_align(size_t size, size_t align);

IC_PUBLIC
void *hunk_alloc_low(size_t size);

IC_PUBLIC
void *hunk_alloc_temp(size_t size);

IC_PUBLIC
void *hunk_alloc_temp_high(size_t size);

IC_PUBLIC
void *hunk_realloc_temp(size_t size);

IC_PUBLIC
void hunk_free_temp(void *buf);

IC_PUBLIC
void hunk_clear_temp(void);

IC_PUBLIC
void hunk_clear_temp_high(void);

/**
 * @brief "Safer" library replacement for `malloc`.

 * Always allocates at least 1 byte of memory, and will fatally exit if
 * unable to allocate the memory requested.
 * 
 * This function is part of a rudimentary memory leak detection system. Make
 * sure all calls to `ic_malloc` are free'd accordingly with `ic_free`.
 *
 * @param[in] size     size in bytes to allocate
 * @param[in] filename filename of the calling function
 * @param[in] function name of the calling function
 * @param[in] line     line number of the calling function
 * @return pointer to allocated memory or NULL if failed
*/
IC_MALLOC
IC_NON_NULL(2, 3)
IC_PUBLIC
void *_ic_malloc(size_t size,
                 _In_z_ const char *filename,
                 _In_z_ const char *function,
                 int line);

/**
 * @def ic_malloc
 * @brief Wrapper for @ref _ic_malloc that automatically inserts the
 * calling function name, and filename/line number for debug purposes.
 */
#define ic_malloc(size) \
    _ic_malloc(size, __FILENAME__, __func__, __LINE__)

/**
 * @brief "Safer" library replacement for `free`.
 *
 * Always checks that the passed pointer is not NULL before freeing.
 *
 * @param[in] ptr memory to free
*/
IC_PUBLIC
void _ic_free(void *ptr);

/**
 * @def ic_free
 * @brief Wrapper for @ref _ic_free.
 */
#define ic_free(ptr) _ic_free(ptr)

/**
 * @brief "Safer" library replacement for `calloc`.
 *
 * Always checks that we allocate at least 1 element of size 1. Will fatally
 * exit if unable to allocate the memory requested.
 * 
 * This function is part of a rudimentary memory leak detection system. Make
 * sure all calls to `ic_calloc` are free'd accordingly with `ic_free`.
 *
 * @param[in] count    number of elements to allocate
 * @param[in] size     size of each element
 * @param[in] filename filename of the calling function
 * @param[in] function name of the calling function
 * @param[in] line     line number of the calling function
 * @return pointer to allocated memory or NULL if failed
*/
IC_MALLOC
IC_NON_NULL(3, 4)
IC_PUBLIC
void *_ic_calloc(size_t count, 
                 size_t size, 
                 _In_z_ const char *filename, 
                 _In_z_ const char *function, 
                 int line);

/**
 * @def ic_calloc
 * @brief Wrapper for @ref _ic_calloc that automatically inserts the
 * calling function name, and filename/line number for debug purposes.
 */
#define ic_calloc(count, size) \
    _ic_calloc(count, size, __FILENAME__, __func__, __LINE__)

/**
 * @brief "Safer" library replacement for `realloc`.
 *
 * Always checks that we have a valid pointer passed, and if the pointer
 * is NULL, will automatically allocate new memory instead of reallocating.
 * Always makes sure to allocate memory of at least 1 byte. Will fatally exit
 * if it cannot reallocate memory.
 * 
 * This function is part of a rudimentary memory leak detection system. Make
 * sure all calls to `ic_realloc` are free'd accordingly with `ic_free`.
 *
 * @param[in] oldptr   old memory to reallocate
 * @param[in] size     new size of memory
 * @param[in] filename filename of the calling function
 * @param[in] function name of the calling function
 * @param[in] line     line number of the calling function
 * @return pointer to allocated memory or NULL if failed
*/
IC_MALLOC
IC_NON_NULL(3, 4)
IC_PUBLIC
void *_ic_realloc(void *oldptr, 
                  size_t size, 
                  _In_z_ const char *filename, 
                  _In_z_ const char *function, 
                  int line);

/**
 * @def ic_realloc
 * @brief Wrapper for @ref _ic_realloc that automatically inserts the
 * calling function name, and filename/line number for debug purposes.
 */
#define ic_realloc(old, size) \
    _ic_realloc(old, size, __FILENAME__, __func__, __LINE__)

/** @} */

#endif /* IOCOD_MEMORY_H */
