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

IC_PUBLIC
bool cmd_remove(const char *name)
{
    struct cmd *cmd;
    struct cmd **back = &cmd_functions;

    while (true) {
        cmd = *back;
        if (cmd == NULL) {
            // command wasn't active
            break;
        }

        if (strcasecmp(name, cmd->name) == 0) {
            *back = cmd->next;

            ic_free(cmd->name);
            ic_free(cmd);
            return true;
        }

        back = &cmd->next;
    }

    return false;
}
