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

#include <stdlib.h>
#include "iocod.h"

IC_PUBLIC
IC_NON_NULL(1)
char *_ic_strdup(const char *s)
{
    #if !defined IC_GNUC_VERSION
    if (s == NULL)
        return NULL;
    #endif

    size_t len = strlen(s) + 1;
    void *new = ic_malloc(len);

    if (new == NULL)
        return NULL;

    return (char *) memcpy(new, s, len);
}
