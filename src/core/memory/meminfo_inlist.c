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

int meminfo_inlist(const char *filename, int line)
{
    struct meminfo *tmp = mem_list;
    int counter = 0;
    size_t len = strlen(filename);

    while (tmp != NULL) {
        if (len == strlen(tmp->filename) &&
            memcmp(filename, tmp->filename, len) == 0 &&
            tmp->line == line) {
            return counter;
        }

        tmp = tmp->next;
        counter++;
    }

    return -1;
}
