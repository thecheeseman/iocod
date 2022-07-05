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

static void cmd_call(struct cmd *cmd)
{
    bool printhelp = false;

    // min args
    if (cmd->argc_min > 0 && (cmd_argc() - 1) < cmd->argc_min) {
        if (cmd->usage != NULL) {
            ic_printf(_("Usage: %s\n"), cmd->usage);
        } else {
            ic_printf(_("'%s' requires at least %d arguments\n"), cmd->name,
                      cmd->argc_min);
        }

        printhelp = true;
    }

    // max args
    if (cmd->argc_max > 0 && (cmd_argc() - 1) > cmd->argc_max) {
        if (cmd->usage != NULL) {
            ic_printf(_("Usage: %s\n"), cmd->usage);
        } else {
            ic_printf(_("'%s' requires at most %d arguments\n"), cmd->name,
                      cmd->argc_max);
        }

        printhelp = true;
    }

    if (printhelp) {
        if (cmd->description != NULL)
            ic_printf("%s\n", cmd->description);

        return;
    }

    // call it
    cmd->function(cmd);
}

IC_PUBLIC
bool cmd_execute_string(const char *text)
{
    cmd_tokenize_string(text);
    if (cmd_argc() == 0)
        return false; // no tokens

    struct cmd *cmd = NULL;
    struct cmd **prev;
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
                cmd_call(cmd);
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

    ic_printf("Unknown command or cvar '%s'\n", cmd_argv(0));
    return false;
}
