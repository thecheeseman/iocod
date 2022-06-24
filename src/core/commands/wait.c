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

#include "cmd_local.h"
#include <stdlib.h>

size_t cmd_wait = 0;

void cmd_wait_f(void)
{
    if (cmd_argc() == 2) {
        long wait = strtol(cmd_argv(1), NULL, 10);

        if (wait < 0)
            cmd_wait = 1; // ignore the argument
        else
            cmd_wait = (size_t) wait;
    } else {
        cmd_wait = 1;
    }
}
