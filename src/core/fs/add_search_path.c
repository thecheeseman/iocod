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

#include "fs_local.h"

IC_NON_NULL(1)
void add_search_path(_In_ searchpath_t *sp)
{
    if (!sp->localized || fs_searchpaths == NULL) {
        sp->next = fs_searchpaths;
        fs_searchpaths = sp;
    } else {
        searchpath_t *a = fs_searchpaths;
        searchpath_t *b;

        do {
            b = a;
            a = b->next;
            if (a == NULL)
                break;
        } while (a->localized == 0);

        sp->next = a;
        b->next = sp;
    }
}
