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

#include "iocod.h"
#include <stdlib.h>

static inline void ic_out_of_memory_error(void)
{
    perror("OUT OF MEMORY ERROR");
    exit(0);
}

IC_MALLOC 
IC_PUBLIC 
void *ic_malloc(size_t size)
{
    void *ptr = malloc(size > 0 ? size : 1);
    if (ptr == NULL)
        ic_out_of_memory_error();

    return ptr;
}

IC_PUBLIC 
void ic_free(void *ptr)
{
    if (ptr != NULL)
        free(ptr);

    ptr = NULL; /* MEM01-C */
}

IC_MALLOC 
IC_PUBLIC 
void *ic_calloc(size_t count, size_t size)
{
    void *ptr = calloc(count > 0 ? count : 1, size > 0 ? size : 1);
    if (ptr == NULL)
        ic_out_of_memory_error();

    return ptr;
}

IC_PUBLIC 
void *ic_realloc(void *oldptr, size_t size)
{
    if (oldptr == NULL)
        return ic_malloc(size);

    void *ptr = realloc(oldptr, size ? size : 1);
    if (ptr == NULL)
        ic_out_of_memory_error();

    return ptr;
}
