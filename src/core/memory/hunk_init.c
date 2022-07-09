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

struct hunk_used hunk_low;
struct hunk_used hunk_high;

byte *hunk_data = NULL;
byte *hunk_data_loc = NULL;
size_t hunk_total;
size_t hunk_allocated;

IC_PUBLIC
void hunk_init(void)
{
    // fs_loadstack

    struct cvar *cv = cv_get("com_hunkmegs", "" DEF_COM_HUNKMEGS_STR,
                             CV_LATCH | CV_ARCHIVE);

    int min = 0;
    if (com_dedicated != NULL && com_dedicated->integer > 0)
        min = MIN_DED_HUNKMEGS;
    else
        min = MIN_COM_HUNKMEGS;

    if (cv->integer < min) {
        hunk_total = (size_t) (1048576 * min);
        ic_printf(_("Minimum com_hunkmegs is %d\n"), min);
    } else {
        hunk_total = (size_t) (cv->integer * 1048576);
    }

    hunk_data = ic_calloc(hunk_total + 31, 1);
    if (hunk_data == NULL) {
        ic_fatal(_("Hunk system failed to allocate %s"),
                 bytes_human_readable(hunk_total + 31));
    }

    // cache align
    hunk_data_loc = hunk_data;
    hunk_data = (byte *) (((intptr_t) hunk_data + 31) & ~31);
    ic_printf(_("Hunk system allocated %s\n"), 
              bytes_human_readable(hunk_total + 31));

    hunk_clear();

    cmd_add("meminfo", hunk_meminfo_f);

    void *test = ic_malloc(240);
}

IC_PUBLIC
void hunk_shutdown(void)
{
    // ic_free(hunk_data_loc);
}
