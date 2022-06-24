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
bool cmd_execute_string(const char *text)
{
    cmd_tokenize_string(text);
    if (cmd_argc() == 0)
        return false; // no tokens

    struct cmd_function *cmd = NULL;
    struct cmd_function **prev;
    for (prev = &cmd_functions; *prev != NULL; prev = &cmd->next) {
        cmd = *prev;

        if (strcasecmp(cmd->name, cmd_argv(0)) == 0) {
            // rearrange the links s othat the command will be
            // near the head of the list next time it is used
            *prev = cmd->next;
            cmd->next = cmd_functions;
            cmd_functions = cmd;

            // perform the action
            if (cmd->function != NULL)
                cmd->function();
            else
                break; // let the VMs handle it

            return true;
        }
    }

    // check cvars
    if (cv_command())
        return true;

    // check client

    // check server

    // check ui

    return false;
}
