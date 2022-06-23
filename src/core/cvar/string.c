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

IC_PUBLIC
char *cv_clear_foreign_chars(const char *value)
{
    if (value == NULL)
        return "";

    static char clean[MAX_VMCVAR_STRING_LEN]; /* should this be larger? */

    int j = 0;
    for (int i = 0; value[i] != '\0'; i++) {
        if ((value[i] & 128) == 0) {
            clean[j] = value[i];
            j++;
        }
    }
    clean[j] = '\0';

    return clean;
}

static char *get_string(const char *name)
{
    if (name == NULL || *name == '\0') {
        log_trace(_("Got NULL value for parameter 'name'\n"));
        return NULL;
    }

    struct cvar *v = cv_find(name);
    if (v == NULL) {
        log_trace(_("Unable to find cvar '%s'\n"), name);
        return NULL;
    }

    return v->string;
}

IC_PUBLIC
IC_NON_NULL(1)
char *cv_get_string(const char *name)
{
    char *str = get_string(name);

    if (str == NULL)
        return "";
    else
        return str;
}

IC_PUBLIC
IC_NON_NULL(1, 2)
void cv_get_string_buffer(const char *name, char *buf, size_t size)
{
    char *str = get_string(name);

    if (str == NULL)
        *buf = '\0';
    else
        strncpyz(buf, str, size);
}

IC_PUBLIC
bool cv_validate_string(const char *s)
{
    if (s == NULL || strchr(s, '\\') || strchr(s, '\"') || strchr(s, ';'))
        return false;

    return true;
}
