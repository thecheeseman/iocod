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

#include <string.h>

#include "cvar_local.h"
#include "common/memory.h"
#include "common/print.h"

/**
 * @brief Handles variable inspection and changing from the console
 * @return True if any variable exists, false otherwise
*/
bool cvar_command(void)
{
    struct cvar *v;

    v = cvar_find_var(cmd_argv(0));
    if (v == NULL)
        return false;

    // perform a variable print or set
    if (cmd_argc() == 1) {
        com_printf("\"%s\" is: \"%s\" default: \"%s\"\n", v->name, v->string, 
                   v->reset_string);

        if (v->latched_string != NULL)
            com_printf("latched: \"%s\"\n", v->latched_string);

        return true;
    }

    cvar_set2(v->name, cmd_argv(1), false);
    return true;
}

/**
 * @brief Toggles a cvar for easy key binding
*/
static void cvar_toggle_f(void)
{
    int v;

    if (cmd_argc() != 2) {
        com_printf("usage: toggle <variable>\n");
        return;
    }

    v = cvar_variable_integer_value(cmd_argv(1));
    v = !v;

    cvar_set2(cmd_argv(1), va("%i", v), false);
}

/**
 * @brief Allows setting and defining of arbitrary cvars from console
*/
static void cvar_set_f(void)
{
    int i, c, l, len;
    char combined[MAX_STRING_TOKENS];

    c = cmd_argc();
    if (c < 3) {
        com_printf("usage: set <variable> <value>\n");
        return;
    }

    combined[0] = 0;
    l = 0;
    for (i = 2; i < c; i++) {
        len = strlen(cmd_argv(i) + 1);
        if (l + len >= MAX_STRING_TOKENS - 2)
            break;

        strcat(combined, cmd_argv(i));
        if (i != c - 1)
            strcat(combined, " ");

        l += len;
    }

    cvar_set2(cmd_argv(1), combined, false);
}

/**
 * @brief Does the same as 'set' but flags it as USERINFO
*/
static void cvar_setu_f(void)
{
    struct cvar *v;

    if (cmd_argc() != 3) {
        com_printf("usage: setu <variable> <value>\n");
        return;
    }

    cvar_set_f();
    v = cvar_find_var(cmd_argv(1));
    if (v == NULL)
        return;

    v->flags |= CVAR_USER_INFO;
}

/**
 * @brief Does the same as 'set' but flags it as SERVERINFO
*/
static void cvar_sets_f(void)
{
    struct cvar *v;

    if (cmd_argc() != 3) {
        com_printf("usage: sets <variable> <value>\n");
        return;
    }

    cvar_set_f();
    v = cvar_find_var(cmd_argv(1));
    if (v == NULL)
        return;

    v->flags |= CVAR_SERVER_INFO;
}

/**
 * @brief Does the same as 'set' but flags it as ARCHIVE
*/
static void cvar_seta_f(void)
{
    struct cvar *v;

    if (cmd_argc() != 3) {
        com_printf("usage: seta <variable> <value>\n");
        return;
    }

    cvar_set_f();
    v = cvar_find_var(cmd_argv(1));
    if (v == NULL)
        return;

    v->flags |= CVAR_ARCHIVE;
}

/**
 * @brief Sets a cvar's flags
 * @author thecheeseman
*/
static void cvar_setf_f(void)
{
    struct cvar *v;
    char *s;
    int flag;
    bool invertnext = false, breakall = false;

    if (cmd_argc() != 3) {
        com_printf("usage: setf <variable> <flags>\n");
        com_printf("  where <flags> are (^)[s]erverinfo, [u]serinfo, [a]rchive\n");
        return;
    }

    v = cvar_find_var(cmd_argv(1));
    if (v == NULL)
        return;

    for (s = cmd_argv(2); *s != '\0'; s++) {
        if (*s == '^') {
            invertnext = true;
            continue;
        }

        flag = 0;
        switch (toupper(*s)) {
            case 'S': flag |= CVAR_SERVER_INFO; break;
            case 'U': flag |= CVAR_USER_INFO; break;
            case 'A': flag |= CVAR_ARCHIVE; break;

                // probably not safe to let people change read-only cvars
                // or screw with cheat protected ones without an sv_cheats check
                // case 'R': flag |= CVAR_ROM; break;
                // case 'I': flag |= CVAR_INIT; break;
                // case 'L': flag |= CVAR_LATCH; break;
                // case 'C': flag |= CVAR_CHEAT; break;
            default:
                com_printf("Unknown flag: %c\n", *s);
                breakall = true;
                break;
        }

        if (breakall)
            break;

        if (invertnext) {
            v->flags ^= flag;
            invertnext = false;
        } else {
            v->flags |= flag;
        }
    }
}

/**
 * @brief Resets a cvar to its default value
*/
static void cvar_reset_f(void)
{
    if (cmd_argc() != 2) {
        com_printf("usage: reset <variable>\n");
        return;
    }

    cvar_reset(cmd_argv(1));
}

/**
 * @brief List all cvars
*/
static void cvar_list_f(void)
{
    struct cvar *var;
    int i, matched;
    char *match;

    if (cmd_argc() > 1)
        match = cmd_argv(1);
    else
        match = NULL;

    com_printf("%-7s %-32s %s\n", " flags", "cvar name", "value");
    if (tty_colors->integer > 0)
        com_printf("\033[90m------- -------------------------------- --------------------------------\033[0m\n");
    else
        com_printf("------- -------------------------------- --------------------------------\n");

    i = 0, matched = 0;
    for (var = cvar_vars; var != NULL; var = var->next, i++) {
        if (match != NULL && !com_filter(match, var->name, false))
            continue;

        if (tty_colors->integer > 0) {
            com_printf("%s%s%s%s%s%s%s %-32s \"%s\"\n",
                (var->flags & CVAR_SERVER_INFO) ? "\033[34mS\033[0m" : " ", // blue
                (var->flags & CVAR_USER_INFO) ? "\033[35mU\033[0m" : " ", // magenta
                (var->flags & CVAR_ROM) ? "\033[32mR\033[0m" : " ", // green
                (var->flags & CVAR_INIT) ? "\033[0mI\033[0m" : " ", // white
                (var->flags & CVAR_ARCHIVE) ? "\033[36mA\033[0m" : " ", // cyan
                (var->flags & CVAR_LATCH) ? "\033[33mL\033[0m" : " ", // yellow
                (var->flags & CVAR_CHEAT) ? "\033[31mC\033[0m" : " ", // red
                var->name, var->string);
        } else {
            com_printf("%s%s%s%s%s%s%s %-32s \"%s\"\n",
                (var->flags & CVAR_SERVER_INFO) ? "S" : " ",
                (var->flags & CVAR_USER_INFO) ? "U" : " ",
                (var->flags & CVAR_ROM) ? "R" : " ",
                (var->flags & CVAR_INIT) ? "I" : " ",
                (var->flags & CVAR_ARCHIVE) ? "A" : " ",
                (var->flags & CVAR_LATCH) ? "L" : " ",
                (var->flags & CVAR_CHEAT) ? "C" : " ",
                var->name, var->string);
        }

        matched++;
    }

    if (match != NULL)
        com_printf("\n%i total cvars, %i matched \"%s\"\n", i, matched, match);
    else
        com_printf("\n%i total cvars\n", i);
    com_printf("%i cvar indexes\n", cvar_num_indexes);
}

/**
 * @brief Set a variables value by another variable's value
*/
static void cvar_set_from_cvar_f(void)
{
    struct cvar *var;
    char *value;

    if (cmd_argc() != 3) {
        com_printf("usage: setfromcvar <variable> <variablein>\n");
        return;
    }

    value = "";
    var = cvar_find_var(cmd_argv(2));
    if (var != NULL)
        value = var->string;

    cvar_set2(cmd_argv(1), value, 0);
}

/**
 * @brief 
*/
static void INCOMPLETE cvar_dump_f(void)
{

}

/**
 * @brief Restart any user variables back to an initial state
*/
static void cvar_restart_f(void)
{
    struct cvar *var, **prev;

    prev = &cvar_vars;
    while (true) {
        var = *prev;
        if (var == NULL)
            break;

        // don't mess with rom values, or some inter-module
        // communication will get broken (com_cl_running, etc)
        if (var->flags & (CVAR_ROM | CVAR_INIT | CVAR_NO_RESTART)) {
            prev = &var->next;
            continue;
        }

        // throw out any variables the user created
        if (var->flags & CVAR_USER_CREATED) {
            *prev = var->next;

            if (var->name != NULL)
                z_free(var->name);

            if (var->string != NULL)
                z_free(var->string);

            if (var->latched_string != NULL)
                z_free(var->latched_string);

            if (var->reset_string != NULL)
                z_free(var->reset_string);

            // clear the var completely, since we
            // can't remove the index from the list
            com_memset(var, 0, sizeof(*var));
            continue;
        }

        cvar_set(var->name, var->reset_string);

        prev = &var->next;
    }
}

/**
 * @brief Add commands
*/
void cvar_add_commands(void)
{
    cmd_add_command("toggle", cvar_toggle_f);
    cmd_add_command("set", cvar_set_f);
    cmd_add_command("setu", cvar_setu_f);
    cmd_add_command("sets", cvar_sets_f);
    cmd_add_command("seta", cvar_seta_f);
    cmd_add_command("setfromcvar", cvar_set_from_cvar_f);
    cmd_add_command("reset", cvar_reset_f);
    cmd_add_command("cvarlist", cvar_list_f);
    cmd_add_command("cvardump", cvar_dump_f);
    cmd_add_command("cvar_restart", cvar_restart_f);

    // new
    cmd_add_command("setf", cvar_setf_f);
}

void cvar_remove_commands(void)
{
    cmd_remove_command("toggle");
    cmd_remove_command("set");
    cmd_remove_command("setu");
    cmd_remove_command("sets");
    cmd_remove_command("seta");
    cmd_remove_command("setfromcvar");
    cmd_remove_command("reset");
    cmd_remove_command("cvarlist");
    cmd_remove_command("cvardump");
    cmd_remove_command("cvar_restart");

    // new
    cmd_remove_command("setf");
}
