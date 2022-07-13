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

#include "com_local.h"
#include <time.h>

void rand_init(void)
{
    unsigned int seed = 0;

    if (sys_random_bytes(sizeof(seed), (byte *) &seed)) {
        log_debug(_("System provided random seed %u"), seed);
        srand(seed);
    } else {
        log_debug(_("System couldn't provide random seed, using time(NULL)"));
        srand(time(NULL));
    }
}
