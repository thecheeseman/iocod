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

static void heapinfo(qbool verbose)
{
    struct meminfo *tmp = mem_list;
    struct meminfo *prev = NULL;
    size_t sum = 0;
    int blocks = 0;

    while (tmp != NULL) {
        prev = tmp;

        sum += tmp->size;
        tmp = tmp->next;
        blocks++;
    }

    ic_printf("  %s total heap allocated in %d blocks\n",
              bytes_human_readable(sum), blocks);

    if (verbose) {
        ic_printf("\n#### address          size         filename:function:line\n");
        ic_printf("---- ---------------- ------------ ----------------------\n");

        tmp = mem_list;
        blocks = 0;
        while (tmp != NULL) {
            prev = tmp;

            ic_printf("%4d %16p %12zu %s:%s:%d\n", blocks, tmp->ptr, tmp->size,
                    tmp->filename, tmp->function, tmp->line);

            tmp = tmp->next;
            blocks++;
        }
        ic_printf("---- ---------------- ------------ ----------------------\n");
    }
}

void heapinfo_f(void)
{
    heapinfo(true);
}

void hunk_meminfo_f(void)
{
    ic_printf("Meminfo:\n");
    ic_printf("  %s total hunk allocated\n",
              bytes_human_readable(hunk_total));
    ic_printf("  %s low permanent\n",
              bytes_human_readable(hunk_low.permanent));

    if (hunk_low.temp != hunk_low.permanent)
        ic_printf("  %s low temp\n", bytes_human_readable(hunk_low.temp));

    ic_printf("  %s high permanent\n",
              bytes_human_readable(hunk_high.permanent));

    if (hunk_high.temp != hunk_high.permanent)
        ic_printf("  %s high temp\n", bytes_human_readable(hunk_high.temp));

    ic_printf("  %s total hunk in use\n",
              bytes_human_readable(hunk_low.permanent + hunk_high.permanent));

    heapinfo(false);
}
