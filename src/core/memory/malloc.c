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

qbool atexit_setup = false;
struct meminfo *mem_list = NULL;

IC_MALLOC
IC_PUBLIC
void *_ic_malloc(size_t size, const char *filename, const char *function, 
                 int line)
{
    void *ptr = malloc(size > 0 ? size : 1);
    if (ptr == NULL) {
        ic_fatal(_("out of memory"));
        return NULL;
    }

    if (!atexit_setup) {
        atexit_setup = true;
        atexit(meminfo_print_leaks);
    }

    int pos = meminfo_inlist(filename, line);
    if (pos == -1) {
        mem_list = meminfo_add(mem_list, ptr, size, filename, function, line);
        if (mem_list == NULL) {
            ic_free(ptr);
            return NULL;
        }
    } else {
        meminfo_edit(pos, size);
    }

    return ptr;
}
