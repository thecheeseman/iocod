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
IC_NON_NULL(1, 2)
qbool com_filter(_In_z_ const char *filter, 
                 _In_z_ const char *name, 
                 qbool casesensitive)
{
    char buf[MAX_TOKEN_CHARS] = { 0 };
    const char *ptr;
    int i;

    while (*filter != '\0') {
        qbool found;

        if (*filter == '*') {
            filter++;
            for (i = 0; *filter != '\0'; i++) {
                if (*filter == '*' || *filter == '?')
                    break;

                buf[i] = *filter;
                filter++;
            }
            buf[i] = '\0';

            if (strlen(buf) > 0) {
                ptr = com_string_contains(name, buf, casesensitive);
                if (ptr == NULL)
                    return false;
                name = ptr + strlen(buf);
            }
        } else if (*filter == '?') {
            filter++;
            name++;
        } else if (*filter == '[' && *(filter + 1) == '[') {
            filter++;
        } else if (*filter == '[') {
            filter++;
            found = false;
            while (*filter != '\0' && !found) {
                if (*filter == ']' && *(filter + 1) != ']')
                    break;

                if (*(filter + 1) == '-' && *(filter + 2) &&
                    (*(filter + 2) != ']' || *(filter + 3) == ']')) {
                    if (casesensitive) {
                        if (*name >= *filter && *name <= *(filter + 2))
                            found = true;
                    } else {
                        if (toupper(*name) >= toupper(*filter) &&
                            toupper(*name) <= toupper(*(filter + 2))) {
                            found = true;
                        }
                    }
                    filter += 3;
                } else {
                    if (casesensitive) {
                        if (*filter == *name)
                            found = true;
                    } else {
                        if (toupper(*filter) == toupper(*name))
                            found = true;
                    }

                    filter++;
                }
            }

            if (!found)
                return false;

            while (*filter != '\0') {
                if (*filter == ']' && *(filter + 1) != ']')
                    break;
                filter++;
            }
            filter++;
            name++;
        } else {
            if (casesensitive) {
                if (*filter != *name)
                    return false;
            } else {
                if (toupper(*filter) != toupper(*name))
                    return false;
            }
            filter++;
            name++;
        }
    }

    return true;
}

IC_PUBLIC
IC_NON_NULL(1, 2)
qbool com_filter_path(_In_z_ const char *filter,
                      _In_z_ const char *name,
                      qbool casesensitive)
{
    size_t i;
    char new_filter[PATH_MAX] = { 0 };

    for (i = 0; i < PATH_MAX - 1 && filter[i] != '\0'; i++) {
        if (filter[i] == '\\' || filter[i] == ':')
            new_filter[i] = '/';
        else
            new_filter[i] = filter[i];
    }
    new_filter[i] = '\0';

    char new_name[PATH_MAX] = { 0 };
    for (i = 0; i < PATH_MAX - 1 && name[i] != '\0'; i++) {
        if (name[i] == '\\' || name[i] == ':')
            new_name[i] = '/';
        else
            new_name[i] = name[i];
    }
    new_name[i] = '\0';

    return com_filter(new_filter, new_name, casesensitive);
}
