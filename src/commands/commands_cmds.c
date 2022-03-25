/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

/**
 * @file commands_cmds.c
 * @date 2022-02-04
*/

#include <string.h>

#include "shared.h"
#include "common.h"

#include "commands/cbuf.h"
#include "common/memory.h"
#include "common/print.h"

// linked from cmd.c
extern struct cmd_function *cmd_functions;

static void cmd_list_f(void)
{
    struct cmd_function *cmd;
    int i, j;
    char *match;

    if (cmd_argc() > 1)
        match = cmd_argv(1);
    else
        match = NULL;

    i = 0;
    for (cmd = cmd_functions; cmd != NULL; cmd = cmd->next) {
        if (match != NULL && !com_filter(match, cmd->name, false))
            continue;

        com_printf("%s ", cmd->name);

        if (cmd->alias_count) {
            com_printf("(aliases: ");

            for (j = 0; j < cmd->alias_count; j++) {
                if (j)
                    com_printf(", %s", cmd->aliases[j]);
                else
                    com_printf("%s", cmd->aliases[j]);
            }

            com_printf(")");
        }

        com_printf("\n");

        i++;
    }

    com_printf("%i commands\n", i);
}

static void cmd_exec_f(void)
{
    char *f;
    int len;
    char filename[MAX_QPATH];

    if (cmd_argc() != 2) {
        com_printf("exec <filename> : execute a script file\n");
        return;
    }

    q_strncpyz(filename, cmd_argv(1), sizeof(filename));
    com_default_extension(filename, sizeof(filename), ".cfg");
    len = fs_read_file(filename, (void **) &f);
    if (f == 0 || len < 0) {
        com_printf("couldn't exec %s\n", cmd_argv(1));
        return;
    }

    com_printf("execing %s\n", cmd_argv(1));
    cbuf_insert_text(f);
    fs_free_file(f);
}

/**
 * @brief Insert the current value of a variable as command text
*/
static void cmd_vstr_f(void)
{
    char *v;

    if (cmd_argc() != 2) {
        com_printf("vstr <variablename> : execute a variable command\n");
        return;
    }

    v = cvar_variable_string(cmd_argv(1));
    cbuf_insert_text(va("%s\n", v));
}

/**
 * @brief Print the rest of the line to console
 */
static void cmd_echo_f(void)
{
    size_t i;

    for (i = 1; i < cmd_argc(); i++)
        com_printf("%s ", cmd_argv(i));
    com_printf("\n");
}

/*
 * Print GPL information as specified by the GPL.
 */
static void cmd_gplinfo_f(void)
{
    com_printf("---------------------------------------------------------------------\n");
    com_printf("iocod\nCopyright(C) 2021 - 2022 thecheeseman\n\n");
    com_printf("This program is free software; you can redistribute it and /or modify\n");
    com_printf("it under the terms of the GNU General Public License as published by\n");
    com_printf("the Free Software Foundation; either version 2 of the License, or \n");
    com_printf("(at your option) any later version.\n\n");
    com_printf("This program is distributed in the hope that it will be useful, \n");
    com_printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    com_printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the\n");
    com_printf("GNU General Public License for more details.\n\n");
    com_printf("You should have received a copy of the GNU General Public License\n");
    com_printf("along with this program.If not, see <https://www.gnu.org/licenses/>.\n");
    com_printf("---------------------------------------------------------------------\n");
}

/*
 * Allows the user to add an alias to an existing command.
 */
static void cmd_alias_f(void)
{
    int c;
    struct cmd_function *cmd, **prev;
    char *cmd_name, *alias;

    if (cmd_argc() != 3) {
        com_printf("usage: alias <command> <alias>\n");
        return;
    }

    cmd_name = cmd_argv(1);
    alias = cmd_argv(2);

    for (prev = &cmd_functions; *prev != NULL; prev = &cmd->next) {
        cmd = *prev;

        if (q_stricmp(alias, cmd->name) == 0) {
            com_printf("Command name '%s' already exists\n", cmd->name);
            return;
        }
    }

    for (prev = &cmd_functions; *prev != NULL; prev = &cmd->next) {
        cmd = *prev;

        if (q_stricmp(cmd_name, cmd->name) == 0) {
            // found it
            c = cmd->alias_count;

            cmd->aliases = realloc(cmd->aliases, 
                                   sizeof(*cmd->aliases) * (c + 1));
            cmd->aliases[c] = z_malloc(sizeof(char *) * (strlen(alias) + 1));
            strcpy(cmd->aliases[c], alias);
            cmd->alias_count++;

            com_printf("Added alias '%s' to command '%s'\n", alias, cmd_name);
            return;
        }
    }

    com_printf("Couldn't find command '%s'\n", cmd_name);
}

void cmd_init(void)
{
    cmd_add_command("cmdlist", cmd_list_f);
    cmd_add_command("echo", cmd_echo_f);
    cmd_add_command("vstr", cmd_vstr_f);
    cmd_add_command("exec", cmd_exec_f);

    cmd_add_command("alias", cmd_alias_f);

    //
    cmd_add_command("gplinfo", cmd_gplinfo_f);
    //
}

void cmd_shutdown(void)
{
    cmd_remove_command("cmdlist");
    cmd_remove_command("echo");
    cmd_remove_command("vstr");
    cmd_remove_command("exec");
    cmd_remove_command("alias");
    cmd_remove_command("gplinfo");

    com_remove_commands();
    cvar_remove_commands();
    fs_remove_commands();

    // net_chan.c
    cmd_remove_command("net_dumpprofile");
}
