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

struct cvar *com_dedicated;
struct cvar *com_sv_running;

IC_PUBLIC
void com_init(void)
{
    ev_init();
    cv_init();
    cbuf_init();
    cmd_init();

    cmd_add("quit", com_quit_f);
    cmd_add("exit", com_quit_f);

    com_dedicated = cv_get("dedicated", "2", CV_ROM);
    com_sv_running = cv_get("sv_running", "1", CV_ROM);
}

IC_PUBLIC
void com_shutdown(void)
{
    
}
