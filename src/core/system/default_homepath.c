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
#include <ShlObj.h>
#endif

static char homepath[MAX_OSPATH] = { 0 };

IC_PUBLIC
char *sys_default_homepath(void)
{
    if (*homepath == '\0') {
        #ifdef IC_PLATFORM_WINDOWS
        wchar_t wpath[MAX_OSPATH] = { 0 };

        if (SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, wpath) == S_OK) {
            char path[MAX_OSPATH] = { 0 };

            utf16_shorten(wpath, path);
            snprintf(homepath, sizeof(homepath), "%s\\iocod", path);
        }
        #else
        char *p;
        if ((p = getenv("HOME")) != NULL) {
            #ifdef IC_PLATFORM_MACOS
            snprintf(homepath, sizeof(homepath),
                        "%s/Library/Application Support/.iocod", p);
            #else
            snprintf(homepath, sizeof(homepath), "%s/.iocod", p);
            #endif
        }
        #endif
    }

    return homepath;
}
