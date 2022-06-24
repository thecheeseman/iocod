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

struct cmd_function *cmd_functions;

IC_PUBLIC
void cmd_init(void)
{
    cmd_add("cmdlist", cmd_list_f);
    cmd_add("echo", cmd_echo_f);
    cmd_add("vstr", cmd_vstr_f);
    cmd_add("wait", cmd_wait_f);
}
