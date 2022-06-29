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
struct cmd *cmd_find(const char *name)
{
    struct cmd *cmd;
    for (cmd = cmd_functions; cmd != NULL; cmd = cmd->next) {
        if (strcasecmp(cmd->name, name) == 0)
            return cmd;
    }

    return NULL;
}
