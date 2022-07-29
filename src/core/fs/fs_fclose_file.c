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
void fs_fclose_file(filehandle f)
{
    filehandle_data_t *handle = &fs_handle[f];

    if (handle->zipfile) {
        // only close unique handles
        // these are if you specify fs_fopen_read() with unique = true
        // otherwise, the filesystem will close the regular pk3 handles
        // at fs_shutdown
        if (handle->handle.unique) {
            mz_zip_reader_end(handle->handle.file.z);
            ic_free(handle->handle.file.z);
        }
    } else {
        if (handle->handle.file.o)
            fclose(handle->handle.file.o);
    }

    memset(handle, 0, sizeof(*handle));
}
