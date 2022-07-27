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

#define MAX_CONSOLE_LINES 32
int num_console_lines;
char *console_lines[MAX_CONSOLE_LINES];

qbool safe_mode(void)
{
    for (int i = 0; i < num_console_lines; i++) {
        cmd_tokenize_string(console_lines[i]);

        if (strcasecmp(cmd_argv(0), "safe") == 0 ||
            strcasecmp(cmd_argv(0), "cvar_restart") == 0) {
            console_lines[i][0] = '\0';
            return true;
        }
    }

    return false;
}

void parse_command_line(_In_z_ char *cmdline)
{
    console_lines[0] = cmdline;
    num_console_lines = 1;

    qbool inquote = false;
    while (*cmdline != '\0') {
        if (*cmdline == '"')
            inquote = !inquote;

        // look for a + separating character
        if ((*cmdline == '+' && !inquote) || *cmdline == '\n' ||
            *cmdline == '\r') {
            if (num_console_lines == MAX_CONSOLE_LINES)
                return;

            console_lines[num_console_lines] = cmdline + 1;
            num_console_lines++;
            *cmdline = '\0';
        }

        cmdline++;
    }
}

IC_PUBLIC
void com_startup_variable(_In_ const char *match)
{
    for (int i = 0; i < num_console_lines; i++) {
        cmd_tokenize_string(console_lines[i]);

        // skip non-set commands
        if (strcasecmp(cmd_argv(0), "set") != 0)
            continue;

        char *s = cmd_argv(1);

        if (match == NULL || strcasecmp(s, match) == 0) {
            struct cvar *cv = cv_find(s);

            if (cv != NULL)
                cv_set2(s, cmd_args_from(2), false);
            else
                cv_get(s, cmd_args_from(2), CV_USER_CREATED);
        }
    }
}

IC_PUBLIC
void com_startup_variables(int count, ...)
{
    va_list ap;
    va_start(ap, count);

    for (int i = 0; i < count; i++) { 
        char *arg = va_arg(ap, char *);
        com_startup_variable(arg);
    }

    va_end(ap);
}

qbool add_startup_commands(void)
{
    qbool added = false;
    for (int i = 0; i < num_console_lines; i++) {
        if (console_lines[i] == NULL || *console_lines[i] == '\0')
            continue;

        if (strncasecmp(console_lines[i], "set ", 4) == 0)
            continue; // skip set commands (already set from startup_variable)

        added = true;
        cbuf_add_text(console_lines[i]);
        cbuf_add_text("\n");
    }

    return added;
}
