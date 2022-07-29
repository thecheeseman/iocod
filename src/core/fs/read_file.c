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

IC_PUBLIC
IC_NON_NULL(1)
i64 fs_read_file(_In_z_ const char *path, 
                 _Out_opt_ void **buffer)
{
    IC_ASSERT(path != NULL && *path != '\0');

    // journal stuff but not really needed

    filehandle handle = 0;
    i64 len = fs_fopen_file_read(path, false, &handle);
    if (handle == 0) {
        if (buffer != NULL)
            *buffer = NULL;

        return -1;
    }

    // just checking if it exists
    if (buffer == NULL) {
        fs_fclose_file(handle);
        return len;
    }

    // TODO: read data

    fs_fclose_file(handle);

    return len;
}
