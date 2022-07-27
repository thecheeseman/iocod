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
IC_NON_NULL(1, 3)
void strncatz(_Out_writes_z_(size) char *dest,
              size_t size,
              _In_z_ const char *src)
{
    IC_ASSERT(size > 0);
    size_t l1 = strlen(dest);
    IC_ASSERT(size > l1);

    strncpyz(dest + l1, src, size - l1);
}
