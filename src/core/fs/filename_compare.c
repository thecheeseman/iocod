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

IC_NON_NULL(1, 2)
int filename_compare(_In_z_ const char *s1, 
                     _In_z_ const char *s2)
{
    int c1 = 0;
    int c2 = 0;

    do {
        c1 = *s1++;
        c2 = *s2++;

        if (c1 >= 'a' && c1 <= 'z')
            c1 -= ('a' - 'A');
        if (c2 >= 'a' && c2 <= 'z')
            c2 -= ('a' - 'A');

        if (c1 == '\\' || c1 == ':')
            c1 = '/';
        if (c2 == '\\' || c2 == ':')
            c2 = '/';

        if (c1 < c2)
            return -1;      // strings not equal

        if (c1 > c2)
            return 1;

    } while (c1);

    return 0;       // strings are equal
}
