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

#include "com_local.h"
#include <ctype.h>

IC_PUBLIC
char *com_string_contains(char *str1, char *str2, bool casesensitive)
{
    int len, i, j;

    len = strlen(str1) - strlen(str2);
    for (i = 0; i <= len; i++, str1++) {
        for (j = 0; str2[j] != '\0'; j++) {
            if (casesensitive) {
                if (str1[j] != str2[j])
                    break;
            } else {
                if (toupper(str1[j]) != toupper(str2[j]))
                    break;
            }
        }

        if (str2[j] == '\0')
            return str1;
    }
    return NULL;
}
