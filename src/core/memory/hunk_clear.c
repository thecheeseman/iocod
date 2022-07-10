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
void hunk_clear_to_start(void)
{
    hunk_low.mark = 0;
    hunk_low.mark2 = 0;
    hunk_low.permanent = 0;
    hunk_low.temp = 0;

    hunk_high.mark = 0;
    hunk_high.mark2 = 0;
    hunk_high.permanent = 0;
    hunk_high.temp = 0;

    hunk_allocated = 0;

    ic_printf(_("Reset the hunk OK\n"));
}

IC_PUBLIC
void hunk_clear(void)
{
    hunk_clear_to_start();
}