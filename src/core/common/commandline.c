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

void parse_command_line(char *cmdline)
{
    console_lines[0] = cmdline;
    num_console_lines = 1;

    bool inquote = false;
    while (*cmdline) {
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

void startup_variable(const char *match)
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

bool add_startup_commands(void)
{
    bool added = false;
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
