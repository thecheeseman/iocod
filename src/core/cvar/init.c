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
    sv_console_lockout = cv_get("sv_console_lockout", "0", 
                                CV_ROM | CV_SYSTEM_INFO);

    cmd_add("cvarlist", cv_list_f);
    cmd_add("print", cv_print_f);
    cmd_add("set", cv_set_f);
    cmd_add("seta", cv_set_f);
    cmd_add("sets", cv_set_f);
    cmd_add("setu", cv_set_f);
    cmd_add("toggle", cv_toggle_f);
    cmd_add("unset", cv_unset_f);
}

IC_PUBLIC
void cv_shutdown(void)
{
    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        if (v->name != NULL)
            ic_free(v->name);

        if (v->string != NULL)
            ic_free(v->string);

        if (v->latched_string != NULL)
            ic_free(v->latched_string);

        if (v->reset_string != NULL)
            ic_free(v->reset_string);
    }
}
