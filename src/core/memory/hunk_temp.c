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
void *hunk_alloc_temp(size_t size)
{
    if (hunk_data == NULL)
        return ic_malloc(size);

    size = PAD(size, sizeof(intptr_t)) + sizeof(struct hunk_header);

    if (hunk_total < hunk_low.temp + hunk_high.temp + size) {
        hunk_meminfo_f();
        ic_error(_("Hunk couldn't allocate %s"),
                 bytes_human_readable(size));
    }

    hunk_low.temp += size;
    void *buf = (void *) (hunk_data + hunk_low.temp);
    struct hunk_header *hdr = (struct hunk_header *) buf;
    buf = (void *) (hdr + 1);

    hdr->magic = HUNK_MAGIC;
    hdr->size = size;

    return buf;
}

IC_PUBLIC
void *hunk_alloc_temp_high(size_t size)
{
    hunk_high.temp += PAD(size, sizeof(intptr_t)) + sizeof(struct hunk_header);

    if (hunk_total < hunk_low.temp + hunk_high.temp) {
        hunk_meminfo_f();
        ic_error(_("Hunk couldn't allocate %s"),
                 bytes_human_readable(size));
    }

    return (hunk_total + hunk_data) - hunk_high.temp;
}

IC_PUBLIC
void *hunk_realloc_temp(size_t size)
{
    hunk_low.temp = size + hunk_low.permanent;

    if (hunk_total < hunk_low.temp + hunk_high.temp) {
        hunk_meminfo_f();
        ic_error(_("Hunk couldn't allocate %s"),
                 bytes_human_readable(size));
    }

    return hunk_low.permanent + hunk_data;
}

IC_PUBLIC
void hunk_free_temp(void *buf)
{
    if (hunk_data == NULL) {
        ic_free(buf);
        return;
    }

    struct hunk_header *hdr = ((struct hunk_header *) buf) - 1;
    if (hdr->magic != (int) HUNK_MAGIC)
        ic_fatal(_("Bad hunk magic"));

    hdr->magic = HUNK_FREE_MAGIC;
    hunk_low.temp -= hdr->size;
}

IC_PUBLIC
void hunk_clear_temp(void)
{
    if (hunk_data == NULL)
        return;

    hunk_low.temp = hunk_low.permanent;
}

IC_PUBLIC
void hunk_clear_temp_high(void)
{
    hunk_high.temp = hunk_high.permanent;
}
