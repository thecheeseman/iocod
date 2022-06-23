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

union cv_value {
    cv_float f;
    cv_int i;
};

static union cv_value get_value(const char *name, bool f)
{
    union cv_value u = { 0 };

    if (name == NULL || *name == '\0') {
        log_trace(_("Got NULL value for parameter 'name'\n"));
        return u;
    }

    struct cvar *v = cv_find(name);
    if (v == NULL) {
        log_trace(_("Unable to find cvar '%s'\n"), name);
        return u;
    }

    if (f)
        u.f = v->value;
    else
        u.i = v->integer;

    return u;
}

IC_PUBLIC
IC_NON_NULL(1)
cv_float cv_get_value(const char *name)
{
    union cv_value u = get_value(name, true);
    return u.f;
}

IC_PUBLIC
IC_NON_NULL(1)
cv_int cv_get_integer(const char *name)
{
    union cv_value u = get_value(name, false);
    return u.i;
}
