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

#include "iocod.h"
#include <stdlib.h>

IC_PUBLIC
IC_NO_RETURN
void sys_exit(int code)
{
    net_shutdown();
    cv_shutdown();
    log_shutdown();
    con_shutdown();
    config_shutdown();

    core_active = false;
    core_quit = true;

    exit(code);
}

IC_PUBLIC
void sys_quit(void)
{
    core_active = false;
    core_quit = true;

    sys_exit(IC_OK);
}
