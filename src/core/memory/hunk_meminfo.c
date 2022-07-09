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
}
