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

#ifndef IC_MEMORY_H
#define IC_MEMORY_H

/**
 * @brief 
 * @param size 
 * @return 
*/
IC_MALLOC 
IC_PUBLIC 
void *ic_malloc(size_t size);

/**
 * @brief 
 * @param ptr 
*/
IC_PUBLIC 
void ic_free(void *ptr);

/**
 * @brief 
 * @param count 
 * @param size 
 * @return 
*/
IC_MALLOC 
IC_PUBLIC 
void *ic_calloc(size_t count, size_t size);

/**
 * @brief 
 * @param oldptr 
 * @param size 
 * @return 
*/
IC_MALLOC 
IC_PUBLIC 
void *ic_realloc(void *oldptr, size_t size);

#endif /* IC_MEMORY_H */
