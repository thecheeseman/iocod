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
void *hunk_alloc_align(size_t size, size_t align)
{
    if (hunk_data == NULL)
        ic_fatal(_("Hunk system not initialized"));

    hunk_high.permanent = ((hunk_high.permanent + size + align) - 1) & ~(align - 1);
    void *ret = ((hunk_data + hunk_total) - hunk_high.permanent);
    hunk_high.temp = hunk_high.permanent;

    if (hunk_total < hunk_low.temp + hunk_high.permanent) {
        hunk_meminfo_f();
        ic_fatal(_("Hunk alloc failed to allocate %s"),
                 bytes_human_readable(size));
    }

    hunk_allocated = hunk_low.permanent + hunk_high.permanent;
    memset(ret, 0, size);
    return ret;
}

IC_PUBLIC
void *hunk_alloc(size_t size)
{
    return hunk_alloc_align(size, HUNK_ALIGN);
}

IC_PUBLIC
void *hunk_alloc_low_align(size_t size, size_t align)
{
    size_t temp = ((hunk_low.permanent + align) - 1) & ~(align - 1);
    hunk_low.permanent = temp + size;
    void *ret = (hunk_data + temp);
    hunk_low.temp = hunk_low.permanent;

    if (hunk_total < hunk_low.permanent + hunk_high.temp) {
        hunk_meminfo_f();
        ic_fatal(_("Hunk couldn't to allocate %s"),
                 bytes_human_readable(size));
    }

    hunk_allocated = hunk_low.permanent + hunk_high.permanent;
    memset(ret, 0, size);
    return ret;
}

IC_PUBLIC
void *hunk_alloc_low(size_t size)
{
    return hunk_alloc_low_align(size, HUNK_ALIGN);
}
