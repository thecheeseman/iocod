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

#include "iocod.h"

struct cvar *cv_cheats;
struct cvar *sv_console_lockout;

extern struct cvar *cvars; /* cv_get.c */
extern void cv_add_commands(void); /* cv_commands.c */

IC_PUBLIC
void cv_init(void)
{
    cv_cheats = cv_get("sv_cheats", "0", CV_ROM | CV_SYSTEM_INFO);
    sv_console_lockout = cv_get("sv_console_lockout", "0", 
                                CV_ROM | CV_SYSTEM_INFO);

    cv_add_commands();
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
