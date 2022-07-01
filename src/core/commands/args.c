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

unsigned int argc;
char *argv[MAX_STRING_TOKENS];

IC_PUBLIC
unsigned int cmd_argc(void)
{
    return argc;
}

IC_PUBLIC
char *cmd_argv(unsigned int arg)
{
    if (arg < argc)
        return argv[arg];

    return "";
}

IC_PUBLIC
void cmd_argv_buffer(unsigned int arg, size_t buflen, char *buf)
{
    if (buf != NULL && buflen > 0)
        strncpyz(buf, cmd_argv(arg), buflen);
}

IC_PUBLIC
char *cmd_args_from(unsigned int arg)
{
    static char args[MAX_STRING_CHARS] = { 0 };

    args[0] = '\0';
    for (unsigned int i = arg; i < argc; i++) {
        strncatz(args, sizeof(args), cmd_argv(i));
        if (i < argc - 1)
            strncatz(args, sizeof(args), " ");
    }

    return args;
}

IC_PUBLIC
char *cmd_args(void)
{
    return cmd_args_from(1);
}

IC_PUBLIC
void cmd_args_buffer(size_t buflen, char *buf)
{
    if (buf != NULL && buflen > 0)
        strncpyz(buf, cmd_args(), buflen);
}
