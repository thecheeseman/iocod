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

struct meminfo *meminfo_add(struct meminfo *info, void *ptr, size_t size, 
                            const char *filename, const char *function, 
                            int line)
{
    struct meminfo *new_info = malloc(sizeof(struct meminfo));
    if (new_info == NULL)
        return NULL;

    new_info->ptr = ptr;
    new_info->size = size;
    new_info->filename = filename;
    new_info->function = function;
    new_info->line = line;
    new_info->next = info;
    new_info->prev = NULL;

    if (info != NULL)
        info->prev = new_info;

    return new_info;
}
