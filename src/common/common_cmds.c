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
 * @file common_cmds.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"

#include "common/error.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "system/shared.h"
#include "strings/stringlib.h"

/**
 * @brief Quit the system
 * @author idsoftware
*/
void com_quit_f(void)
{
    if (!com_error_entered()) {
        sv_shutdown("Server quit\n");
        sv_remove_operator_commands();
        cl_shutdown();
        com_shutdown();
        fs_shutdown(true);
    }

    sys_quit();
}

/**
 * @brief Test errors
 * @author idsoftware
*/
static void com_error_f(void)
{
    if (cmd_argc() > 1)
        com_error(ERR_DROP, "testing drop error");
    else
        com_error(ERR_FATAL, "testing fatal error");
}

/**
 * @brief Freeze the system for x amount of seconds 
 * @author idsoftware
*/
static void com_freeze_f(void)
{
    float s;
    int start, now;

    if (cmd_argc() != 2) {
        com_printf("freeze <seconds>\n");
        return;
    }

    s = (float) atof(cmd_argv(1));
    start = com_milliseconds();
    while (true) {
        now = com_milliseconds();
        if ((now - start) * 0.001 > s)
            break;
    }
}

/**
 * @brief Cause a segfault (should gracefully exit)
 * @author idsoftware
*/
static void com_crash_f(void)
{
    *(int *) 0 = 0x12345678;
}

static void com_writeconfig_f(void)
{
    char filename[MAX_QPATH];

    if (cmd_argc() != 2) {
        com_printf("usage: writeconfig <filename>\n");
        return;
    }

    strncpyz(filename, cmd_argv(1), sizeof(filename));
    com_default_extension(filename, sizeof(filename), ".cfg");
    com_printf("Writing %s.\n", filename);
    com_write_config_to_file(filename);
}

static void com_writedefaults_f(void)
{
    char filename[MAX_QPATH];

    if (cmd_argc() != 2) {
        com_printf("usage: writedefaults <filename>\n");
        return;
    }

    strncpyz(filename, cmd_argv(1), sizeof(filename));
    com_default_extension(filename, sizeof(filename), ".cfg");
    com_printf("Writing %s.\n", filename);
    com_write_defaults_to_file(filename);
}


/**
 * @brief Add common commands, moved here from common.c for clarity
*/
void com_add_commands(void)
{
    if (com_developer != NULL && com_developer->integer > 0) {
        cmd_add_command("error", com_error_f);
        cmd_add_command("freeze", com_freeze_f);
        cmd_add_command("crash", com_crash_f);
    }
    cmd_add_command("quit", com_quit_f);
    cmd_add_alias("quit", "exit");
    cmd_add_command("writeconfig", com_writeconfig_f);
    cmd_add_command("writedefaults", com_writedefaults_f);
}

void com_remove_commands(void)
{
    if (com_developer != NULL && com_developer->integer > 0) {
        cmd_remove_command("error");
        cmd_remove_command("freeze");
        cmd_remove_command("crash");
    }

    cmd_remove_command("quit");
    cmd_remove_command("writeconfig");
    cmd_remove_command("writedefaults");
}
