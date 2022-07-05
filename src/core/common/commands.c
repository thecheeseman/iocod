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
#include <stdlib.h>

static void com_setenv_f(struct cmd *self)
{
    UNUSED_PARAM(self);

    int argc = cmd_argc();
    char *name = cmd_argv(1);

    if (argc > 2) {
        char *value = cmd_args_from(2);

        #ifdef IC_PLATFORM_WINDOWS
        if (value != NULL && *value != '\0')
            _putenv(va("%s=%s", name, value));
        else
            _putenv(va("%s=", name));
        #else
        if (value != NULL && *value != '\0')
            setenv(name, value, 1);
        else
            unsetenv(name);
        #endif
    } else {
        char *env = getenv(name);

        if (env != NULL)
            ic_printf("%s = %s\n", name, env);
        else
            ic_printf("%s undefined\n", name);
    }
}

static void com_error_f(struct cmd *self)
{
    UNUSED_PARAM(self);

    if (cmd_argc() == 2)
        ic_fatal("FATAL ERROR TEST");
    else
        ic_error("ERROR TEST");
}

static void com_crash_f(struct cmd *self)
{
    UNUSED_PARAM(self);

    *(volatile int *) 0 = 0x12345678;
}

static void com_freeze_f(struct cmd *self)
{
    UNUSED_PARAM(self);

    float s = strtod(cmd_argv(1), NULL);

    int start = com_milliseconds();
    while (true) {
        int now = com_milliseconds();
        if ((now - start) * 0.001 > s)
            break;
    }
}

static void com_quit_f(struct cmd *self)
{
    UNUSED_PARAM(self);

    // TODO: handle errors here
    if (!error_entered) {
        // sv shutdown
        // cl shutdown
        com_shutdown();
    }

    sys_quit();
}

void add_common_commands(void)
{
    cmd_add2("setenv", com_setenv_f, 1, 2,
             _("setenv <variable> [<value>]"),
             _("Set or retreive an environment variable."));

    if (com_developer != NULL && com_developer->integer > 0) {
        cmd_add2("error", com_error_f, 0, 1,
                 _("error"),
                 _("Cause an error."));
        cmd_add2("crash", com_crash_f, 0, 0,
                 _("crash"),
                 _("Crash the game intentionally."));
        cmd_add2("freeze", com_freeze_f, 1, 1,
                 _("freeze <time>"),
                 _("Test freeze the game for a given number of seconds."));
    }

    cmd_add2("quit", com_quit_f, 0, 0,
             _("quit"),
             _("Quit the game."));
    cmd_add2("exit", com_quit_f, 0, 0,
             _("exit"),
             _("Quit the game."));
}
