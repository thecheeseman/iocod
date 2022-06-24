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

#include <stdio.h>

#include "cvar_local.h"

#define fs_printf(f, s, ...) ic_printf(s, __VA_ARGS__)

IC_PUBLIC
bool cv_write_defaults(filehandle f)
{
    UNUSED_PARAM(f);

    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        /* don't write cd key */
        if (strcasecmp(v->name, "cl_cdkey") == 0)
            continue;

        /* don't write out cvars with these flags */
        #define CV_MASK (CV_ROM | CV_USER_CREATED | CV_CHEAT | CV_4096)
        if ((v->flags & CV_MASK) == 0) {
            char buf[1024];
            snprintfz(buf, sizeof(buf), "set %s \"%s\"", v->name, 
                      v->reset_string);

            fs_printf(f, "%s\n", buf);
        }
    }

    return true;
}

IC_PUBLIC
bool cv_write_variables(filehandle f)
{
    UNUSED_PARAM(f);

    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        /* don't write cd key */
        if (strcasecmp(v->name, "cl_cdkey") == 0)
            continue;

        /* only write archived cvars */
        if (v->flags & CV_ARCHIVE) {
            char buf[1024];

            if (v->latched_string != NULL) {
                snprintf(buf, sizeof(buf), "seta %s \"%s\"", v->name,
                         v->latched_string);
            } else {
                snprintfz(buf, sizeof(buf), "seta %s \"%s\"", v->name,
                          v->string);
            }

            fs_printf(f, "%s\n", buf);
        }
    }

    return true;
}
