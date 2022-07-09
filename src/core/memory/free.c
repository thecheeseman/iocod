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
void _ic_free(void *ptr)
{
    if (ptr == NULL)
        return;
    
    struct meminfo *tmp = mem_list;
    struct meminfo *tofree = NULL;
    struct meminfo *prev = NULL;

    if (tmp->ptr == ptr) {
        tofree = tmp;
        mem_list = tmp->next;
        free(tofree->ptr);
        free(tofree);

        if (mem_list)
            mem_list->prev = NULL;

        ptr = NULL; /* MEM01-C */
        return;
    }

    while (tmp != NULL) {
        if (tmp->ptr == ptr) {
            tofree = tmp;
            tmp = tmp->next;
            prev = tofree->prev;

            if (prev != NULL)
                prev->next = tmp;

            if (tmp != NULL)
                tmp->prev = prev;

            free(tofree->ptr);
            if (tofree == mem_list)
                mem_list = NULL;
            free(tofree);
            ptr = NULL;
            return;
        }

        tmp = tmp->next;
    }
}
