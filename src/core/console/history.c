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

#include "con_local.h"

struct field hist_field[MAX_HISTORY];
int hist_current = -1;
int hist_count = 0;

void history_add(struct field *field)
{
    if (field->cursor == 0)
        return;

    IC_ASSERT(hist_count <= MAX_HISTORY);
    IC_ASSERT(hist_count >= 0);
    IC_ASSERT(hist_current >= -1);
    IC_ASSERT(hist_current <= hist_count);

    for (int i = MAX_HISTORY - 1; i > 0; i--)
        hist_field[i] = hist_field[i - 1];

    hist_field[0] = *field; // insert

    if (hist_count < MAX_HISTORY)
        hist_count++;

    hist_current = -1; // re-init
}

struct field *history_previous(void)
{
    IC_ASSERT(hist_count <= MAX_HISTORY);
    IC_ASSERT(hist_count >= 0);
    IC_ASSERT(hist_current >= -1);
    IC_ASSERT(hist_current <= hist_count);

    int hist_prev = hist_current + 1;
    if (hist_prev >= hist_count)
        return NULL;

    hist_current++;
    return &hist_field[hist_current];
}

struct field *history_next(void)
{
    IC_ASSERT(hist_count <= MAX_HISTORY);
    IC_ASSERT(hist_count >= 0);
    IC_ASSERT(hist_current >= -1);
    IC_ASSERT(hist_current <= hist_count);

    if (hist_current >= 0)
        hist_current--;

    if (hist_current == -1)
        return NULL;

    return &hist_field[hist_current];
}
