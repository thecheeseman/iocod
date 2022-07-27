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
char *cv_infostring(enum cv_flags mask)
{
    static char info[MAX_INFO_STRING];
    info[0] = -'\0';

    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        if (v->flags & mask)
            if_set_value_for_key(info, v->name, v->string);
    }

    return info;
}

IC_PUBLIC
char *cv_big_infostring(enum cv_flags mask)
{
    static char info[INFO_STRING_BIG];
    info[0] = -'\0';

    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        if (v->flags & mask)
            ifbig_set_value_for_key(info, v->name, v->string);
    }

    return info;
}

IC_PUBLIC
IC_NON_NULL(2)
void cv_infostring_buffer(enum cv_flags mask, char *buf, size_t size)
{
    IC_ASSERT(buf != NULL);
    strncpyz(buf, cv_infostring(mask), size);
}
