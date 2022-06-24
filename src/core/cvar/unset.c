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
struct cvar *cv_unset(struct cvar *cv)
{
    struct cvar *next = cv->next;

    // note what types of cvars have been modified
    modified_flags |= cv->flags;

    ic_free(cv->name);
    ic_free(cv->string);
    ic_free(cv->latched_string);
    ic_free(cv->reset_string);
    ic_free(cv->description);

    if (cv->prev != NULL)
        cv->prev->next = cv->next;
    else
        cvars = cv->next;

    if (cv->next != NULL)
        cv->next->prev = cv->prev;

    if (cv->hash_prev != NULL)
        cv->hash_prev->hash_next = cv->hash_next;
    else
        hashtable[cv->hash_index] = cv->hash_next;

    if (cv->hash_next)
        cv->hash_next->hash_prev = cv->hash_prev;

    memset(cv, 0, sizeof(*cv));

    return next;
}

void cv_unset_f(void)
{
    if (cmd_argc() != 2) {
        ic_printf(_("Usage: unset <varname>\n"));
        return;
    }

    struct cvar *cv = cv_find(cmd_argv(1));
    if (cv == NULL)
        return;

    if (cv->flags & CV_USER_CREATED)
        cv_unset(cv);
    else
        ic_printf(_("Variable '%s' is not user created.\n"), cv->name);
}
