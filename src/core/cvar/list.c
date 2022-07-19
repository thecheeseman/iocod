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

void cv_list_f(void)
{
    char *match = NULL;
    if (cmd_argc() > 1)
        match = cmd_argv(1);

    ic_printf("%-7s %-32s %s\n", "flags", "cvar name", "value");
    ic_printf("------- -------------------------------- "
              "--------------------------------\n");

    struct cvar *cv;
    int num_cvars = 0;
    int matched = 0;
    for (cv = cvars; cv != NULL; cv = cv->next, num_cvars++) {
        if (match != NULL && !com_filter(match, cv->name, false))
            continue;

        ic_printf("%s%s%s%s%s%s%s %-32s \"%s\"\n",
                  (cv->flags & CV_SERVER_INFO) ? "^5S^7" : " ",
                  (cv->flags & CV_USER_INFO) ? "^6U^7" : " ",
                  (cv->flags & CV_ROM) ? "^2R^7" : " ",
                  (cv->flags & CV_INIT) ? "^7I" : " ",
                  (cv->flags & CV_ARCHIVE) ? "^5A^7" : " ",
                  (cv->flags & CV_LATCH) ? "^3L^7" : " ",
                  (cv->flags & CV_CHEAT) ? "^1C^7" : " ",
                  cv->name, cv->string);

        matched++;
    }

    ic_printf("\n%i total cvars", num_cvars);

    if (match != NULL)
        ic_printf(", %i matched \"%s\"", matched, match);

    ic_printf("\n%lu cvar indexes\n", num_indexes);
}
