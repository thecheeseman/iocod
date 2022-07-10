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
#include <dlfcn.h>
#endif

enum {
    ERR_OK,
    ERR_NULL_PATH,
    ERR_NULL_HANDLE,
    ERR_NULL_FUNCTION
};

static char *errtext[] = {
    NULL,
    "NULL path",
    "NULL handle",
    "NULL function name"
};

static int localerr = -1;

IC_PUBLIC
char *sys_library_error(void)
{
    // handle local error codes first
    if (localerr != -1) {
        int errcode = localerr;
        localerr = -1;

        return errtext[errcode];
    }

    // system error codes
    #ifdef IC_PLATFORM_WINDOWS
    DWORD err = GetLastError();
    static wchar_t wmsg[1024] = { 0 };
    static char msg[1024] = { 0 };

    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL,
                   err,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   wmsg,
                   sizeof(wmsg),
                   NULL);

    utf16_shorten(wmsg, msg);
    return msg;
    #else
    return dlerror();
    #endif
}

IC_PUBLIC
bool sys_library_load(const char *path, void **handle)
{
    *handle = NULL;

    if (path == NULL) {
        localerr = ERR_NULL_PATH;
        return false;
    }

    // append system extension if not provided
    char newpath[PATH_MAX] = { 0 };
    strncpyz(newpath, path, sizeof(newpath));
    ic_append_extension(newpath, sizeof(newpath), "." IC_PLATFORM_DLL);

    // load library
    #ifdef IC_PLATFORM_WINDOWS
    wchar_t wpath[PATH_MAX] = { 0 };
    utf8_widen(newpath, wpath);
    *handle = (void *) LoadLibraryW(wpath);
    #else
    *handle = dlopen(newpath, RTLD_NOW);
    #endif

    return (*handle != NULL);
}

IC_PUBLIC
bool sys_library_close(void *handle)
{
    #ifdef IC_PLATFORM_WINDOWS
    return FreeLibrary((HMODULE) handle);
    #else
    return (dlclose(handle) == 0);
    #endif
}

IC_PUBLIC
bool sys_library_load_symbol(void *handle, const char *fn, void **symbol)
{
    *symbol = NULL;

    if (handle == NULL) {
        localerr = ERR_NULL_HANDLE;
        return false;
    }

    if (fn == NULL) {
        localerr = ERR_NULL_FUNCTION;
        return false;
    }

    #ifdef IC_PLATFORM_WINDOWS
    *symbol = (void *) GetProcAddress((HMODULE) handle, fn);
    #else
    *symbol = dlsym(handle, fn);
    #endif

    return (*symbol != NULL);
}
