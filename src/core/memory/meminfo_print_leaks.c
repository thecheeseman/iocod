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
#include <time.h>

void meminfo_print_leaks(void)
{
    if (mem_list == NULL)
        return; // no leaks

    fprintf(stderr, _("Memory leaks detected, check leaks.log for details\n"));

    FILE *out = stderr;
    FILE *fp = fopen("leaks.log", "a+");
    if (fp != NULL) {
        setvbuf(fp, NULL, _IONBF, 0);
        out = fp;
    }

    time_t t;
    time(&t);

    struct tm *tm_info = localtime(&t);
    char tmbuf[64];
    strftime(tmbuf, sizeof(tmbuf), "%F %T", tm_info);

    struct meminfo *tmp = mem_list;
    struct meminfo *prev = NULL;
    size_t sum = 0;
    int blocks = 0;

    fprintf(out, "----------------------------------------"
            "----------------------------------------\n");
    fprintf(out, "Leak report: %s\n", tmbuf);
    
    fprintf(out, "----------------------------------------\n");
    fprintf(out, "Leaks:\n");
    fprintf(out, "#### address          size         filename:function:line\n");
    fprintf(out, "---- ---------------- ------------ ----------------------\n");
    while (tmp != NULL) {
        prev = tmp;

        fprintf(out, "%4d %16p %12zu %s:%s:%d\n", blocks, tmp->ptr, tmp->size, 
                tmp->filename, tmp->function, tmp->line);

        sum += tmp->size;
        tmp = tmp->next;
        // ic_free(prev);
        blocks++;
    }
    fprintf(out, "---- ---------------- ------------ ----------------------\n");
    fprintf(out, "%4d                  %12zu (%s)\n", blocks, sum,
            bytes_human_readable(sum));
    fprintf(out, "----------------------------------------"
            "----------------------------------------\n");

    if (fp != NULL)
        fclose(fp);
}
