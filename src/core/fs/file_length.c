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
i64 fs_filep_length(_In_ FILE *handle)
{
    long pos = ftell(handle);
    fseek(handle, 0, SEEK_END);
    long end = ftell(handle);
    fseek(handle, pos, SEEK_SET);

    return end;
}

i64 fs_file_length(filehandle f)
{
    FILE *handle = file_for_handle(f);
    if (handle == NULL)
        return -1;
        
    return fs_filep_length(handle);
}
