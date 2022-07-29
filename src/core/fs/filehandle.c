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

filehandle handle_for_file()
{
    for (u32 i = 1; i < MAX_FILE_HANDLES; i++) {
        if (fs_handle[i].handle.file.o == NULL)
            return i;
    }

    ic_error(_("no free handles"));
    return 0;
}

FILE *file_for_handle(filehandle f)
{
    IC_ASSERT(f < MAX_FILE_HANDLES);
    IC_ASSERT(fs_handle[f].zipfile == false);
    IC_ASSERT(fs_handle[f].handle.file.o != NULL);

    return fs_handle[f].handle.file.o;
}
