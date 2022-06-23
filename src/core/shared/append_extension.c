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

#ifndef PATH_MAX
#define PATH_MAX 256
#endif

IC_PUBLIC
bool ic_append_extension(char *path, size_t size, const char *ext)
{
    if (path == NULL || size == 0 || ext == NULL)
        return false;

    const char *dot = strrchr(path, '.');
    if (dot != NULL && strcmp(dot, ext) == 0)
        return true; // already has the correct extension

    const char *slash = strrchr(path, '/');
    if (slash != NULL && path[strlen(path) - 1] == '/')
        return true; // path ends with a '/'

    // append correct extension
    char oldpath[PATH_MAX];
    strncpyz(oldpath, path, sizeof(oldpath));
    snprintf(path, size, "%s%s", oldpath, ext);

    return true;
}
