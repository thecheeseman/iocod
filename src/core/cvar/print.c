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

IC_PUBLIC
IC_NON_NULL(1)
void cv_print(_In_ cvar_t *cv)
{
    IC_ASSERT(cv != NULL);
    ic_printf(_("\"%s\" is set to: \"%s^7\""), cv->name, cv->string);

    if ((cv->flags & CV_ROM) == 0) {
        if (strcasecmp(cv->string, cv->reset_string) == 0)
            ic_printf(_(" (default)"));
        else
            ic_printf(_(" (default setting: \"%s^7\")"), cv->reset_string);
    }

    ic_printf("\n");

    if (cv->latched_string != NULL)
        ic_printf(_("Latched string: \"%s^7\""), cv->latched_string);

    if (cv->description != NULL)
        ic_printf(_("Description: %s\n"), cv->description);
}

void cv_print_f(void)
{
    char *name = cmd_argv(1);
    struct cvar *cv = cv_find(name);

    if (cv != NULL)
        cv_print(cv);
    else
        ic_printf(_("Cvar '%s' does not exist.\n"), name);
}
