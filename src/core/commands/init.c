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

struct cmd *cmd_functions;

static void cmd_echo_f(struct cmd *self)
{
    UNUSED_PARAM(self);

    ic_printf("%s\n", cmd_args());
}

static void cmd_help_f(struct cmd *self)
{
    UNUSED_PARAM(self);
}

static void cmd_list_f(struct cmd *self)
{
    UNUSED_PARAM(self);

    char *match = NULL;

    if (cmd_argc() > 1)
        match = cmd_argv(1);

    struct cmd *cmd;
    int num_cmds = 0;
    for (cmd = cmd_functions; cmd != NULL; cmd = cmd->next) {
        if (match != NULL && !com_filter(match, cmd->name, false))
            continue;

        ic_printf("%s\n", cmd->name);
        num_cmds++;
    }

    ic_printf(_("%i commands\n"), num_cmds);
}

static void cmd_vstr_f(struct cmd *self)
{
    UNUSED_PARAM(self);

    char *v = cv_get_string(cmd_argv(1));
    cbuf_insert_text(va("%s\n", v));
}

size_t cmd_wait = 0;

static void cmd_wait_f(struct cmd *self)
{
    UNUSED_PARAM(self);

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

IC_PUBLIC
void cmd_init(void)
{
    cmd_add2("cmdlist", cmd_list_f, 0, 1,
             _("cmdlist [match]"), 
             _("List all available commands."));

    cmd_add2("echo", cmd_echo_f, 1, 0, 
             _("echo <string>"), 
             _("Echo a string to the console output."));

    cmd_add2("help", cmd_help_f, 0, 0,
             _("help [<topic or command>]"),
             _("Lookup usage or information about a given topic or command."));

    cmd_add2("vstr", cmd_vstr_f, 1, 1, 
             _("vstr <variable_name>"),
             _("Execute a cvar as a command."));

    cmd_add2("wait", cmd_wait_f, 1, 1,
             _("wait <time>"),
             _("Wait specified amount of frames."));
}

IC_PUBLIC
void cmd_shutdown(void)
{
    struct cmd *head = cmd_functions;
    struct cmd *cmd;

    while (head != NULL) {
        cmd = head;
        head = cmd->next;
        
        ic_free(cmd->name);
        ic_free(cmd->usage);
        ic_free(cmd->description);
        ic_free(cmd);
    }
}
