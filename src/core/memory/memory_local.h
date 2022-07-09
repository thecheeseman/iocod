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

#ifndef MEMORY_LOCAL_H
#define MEMORY_LOCAL_H

#include "iocod.h"

#define	HUNK_MAGIC	0x89537892
#define	HUNK_FREE_MAGIC	0x895
#define PAD(base, alignment) (((base)+(alignment)-1) & ~((alignment)-1))

#define MIN_DED_HUNKMEGS 1
#define MIN_COM_HUNKMEGS 64
#define DEF_COM_HUNKMEGS 128
#define DEF_COM_HUNKMEGS_STR IC_STRINGIFY(DEF_COM_HUNKMEGS)
#define HUNK_ALIGN 32
#define HUNK_TEMP_ALIGN 16

struct hunk_header {
    int magic;
    size_t size;
};

struct hunk_used {
    size_t mark;
    size_t mark2;
    size_t permanent;
    size_t temp;
};

extern struct hunk_used hunk_low;
extern struct hunk_used hunk_high;

extern byte *hunk_data;
extern size_t hunk_total;
extern size_t hunk_allocated;

#define BTOGB(v) ((float)((float) (v) / 1073741824))
#define BTOMB(v) ((float)((float) (v) / 1048576))
#define BTOKB(v) ((float)((float) (v) / 1024))

char *bytes_human_readable(size_t size);
void hunk_meminfo_f(void);

#endif /* MEMORY_LOCAL_H */
