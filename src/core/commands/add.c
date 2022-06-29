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
bool cmd_add2(const char *name, void (*function)(struct cmd *), int min,
              int max, const char *usage, const char *description)
{
    if (cmd_find(name)) {
        if (function != NULL)
            log_warn(_("Command '%s' already defined"), name);

        return false;
    }

    struct cmd *cmd = ic_malloc(sizeof(struct cmd));
    cmd->name = strdup(name);
    cmd->function = function;
    cmd->next = cmd_functions;

    cmd->argc_min = min;
    cmd->argc_max = max;

    cmd->usage = NULL;
    if (usage != NULL)
        cmd->usage = strdup(usage);

    cmd->description = NULL;
    if (description != NULL)
        cmd->description = strdup(description);

    cmd_functions = cmd;

    return true;
}

IC_PUBLIC
bool cmd_add(const char *name, void (*function)(struct cmd *))
{
    return cmd_add2(name, function, 0, 0, NULL, NULL);
}
