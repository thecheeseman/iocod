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

#include "memory_local.h"

IC_PUBLIC 
void *_ic_realloc(void *oldptr, size_t size, const char *filename, 
                  const char *function, int line)
{
    if (oldptr == NULL)
        return ic_malloc(size);

    void *ptr = realloc(oldptr, size ? size : 1);
    if (ptr == NULL) {
        ic_fatal(_("out of memory"));
        return NULL;
    }

    if (!atexit_setup) {
        atexit_setup = true;
        atexit(meminfo_print_leaks);
    }

    // find old pointer and update
    struct meminfo *tmp = mem_list;
    while (tmp != NULL) {
        if (tmp->ptr == oldptr) {
            tmp->ptr = ptr;
            tmp->size = size;
            tmp->filename = filename;
            tmp->function = function;
            tmp->line = line;
            return ptr;
        }
        tmp = tmp->next;
    }
    
    // this should not happen
    mem_list = meminfo_add(mem_list, ptr, size, filename, function, line);
    if (mem_list == NULL) {
        ic_free(ptr);
        return NULL;
    }

    return ptr;
}
