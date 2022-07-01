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

#include "cvar_local.h"

struct cvar *cv_cheats;
struct cvar *sv_console_lockout;

IC_PUBLIC
void cv_init(void)
{
    cv_cheats = cv_get("sv_cheats", "0", CV_ROM | CV_SYSTEM_INFO);
    cv_set_description(cv_cheats, _("Enable or disable cheat cvars."));

    sv_console_lockout = cv_get("sv_console_lockout", "0",
                                CV_ROM | CV_SYSTEM_INFO);
    cv_set_description(sv_console_lockout, _("Disable console input."));

    cmd_add2("cvarlist", cv_list_f, 0, 1,
             _("cvarlist [match]"),
             _("List all cvars."));
    cmd_add2("print", cv_print_f, 1, 1,
             _("print <variable>"),
             _("Print the value of a cvar."));
    cmd_add2("set", cv_set_f, 1, 2,
             _("set <variable> <value>"),
             _("Set a console variable."));
    cmd_add2("seta", cv_set_f, 1, 2,
             _("seta <variable> <value>"),
             _("Set a console variable (ARCHIVE)."));
    cmd_add2("sets", cv_set_f, 1, 2,
             _("sets <variable> <value>"),
             _("Set a console variable (SERVER INFO)."));
    cmd_add2("setu", cv_set_f, 1, 2,
             _("setu <variable> <value>"),
             _("Set a console variable (USER INFO)."));
    cmd_add2("toggle", cv_toggle_f, 1, 0,
             _("toggle <variable> [value1, value2...]"),
             _("Toggle a console variable."));
    cmd_add2("unset", cv_unset_f, 1, 1,
             _("unset <variable>"),
             _("Unset a console variable."));
}

IC_PUBLIC
void cv_shutdown(void)
{
    struct cvar *head = cvars;
    struct cvar *v;

    while (head != NULL) {
        v = head;
        head = v->next;

        ic_free(v->name);
        ic_free(v->string);
        ic_free(v->latched_string);
        ic_free(v->reset_string);
        ic_free(v->description);
    }
}
