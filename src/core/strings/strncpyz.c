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
IC_NON_NULL(1, 2)
void strncpyz(char *dest, const char *src, size_t destsize)
{
    #if !defined IC_GNUC_VERSION
    if (src == NULL)
        return; // com_error(ERR_FATAL, "strncpyz: NULL src");
    #endif

    if (destsize < 1)
        return; // com_error(ERR_FATAL, "strncpyz: destsize < 1");

    strncpy(dest, src, destsize - 1);
    dest[destsize - 1] = 0;
}
