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

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dirent.h>
#endif

IC_PUBLIC
IC_NON_NULL(1)
qbool sys_directory_has_contents(_In_z_ const char *path)
{
    IC_ASSERT(path != NULL);
    IC_ASSERT(*path != '\0');

    qbool ret = false;

    #ifdef IC_PLATFORM_WINDOWS
    HANDLE find;
    WIN32_FIND_DATAW data = { 0 };
    wchar_t wpath[MAX_OSPATHW] = { 0 };
    wchar_t wprepend[MAX_OSPATHW] = { 0 };

    utf8_widen(path, wpath);
    /*
        MSDN:
        In the ANSI version of this function, the name is limited to 
        MAX_PATH characters. To extend this limit to 32,767 wide characters, 
        call the Unicode version of the function and prepend "\\?\" 
        to the path 
    */
    _snwprintf(wprepend, MAX_OSPATHW, L"\\\\?\\%s\\*", wpath);
    wprepend[MAX_OSPATHW - 1] = '\0';

    find = FindFirstFileW(wprepend, &data);
    if (find == INVALID_HANDLE_VALUE)
        return false;

    do {
        // skip directories
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        ret = true;
        break;
    } while (FindNextFileW(find, &data) != 0);

    FindClose(find);
    #else
    DIR *fdir;
    struct dirent *d;

    if (!(fdir = opendir(path)))
        return false;

    while ((d = readdir(fdir)) != NULL) {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;

        ret = true;
        break;
    }

    closedir(fdir);
    #endif

    return ret;
}
