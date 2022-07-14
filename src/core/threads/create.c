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

#include "threads_local.h"

IC_PUBLIC
threadptr_t thread_create(int (*function)(void *), void *data, const char *name,
                          size_t stack_size)
{
    #ifdef IC_PLATFORM_WINDOWS
    DWORD threadid;
    HANDLE handle = CreateThread(NULL, stack_size > 0 ? stack_size : 0U,
                                 (LPTHREAD_START_ROUTINE) function, data,
                                 0, &threadid);

    if (handle == NULL)
        return NULL;

    if (name != NULL && IsDebuggerPresent()) {
        HRESULT r;
        wchar_t threadname[256] = { 0 };
        utf8_widen(name, threadname);
        
        r = SetThreadDescription(handle, threadname);
    }

    return (threadptr_t) handle;
    #else
    pthread_t thread;

    if (pthread_create(&thread, NULL, (void *(*)(void *)) function, data) != 0)
        return NULL;

    #if 0
    if (name != NULL)
        pthread_setname_np(thread, name);
    #endif

    return (threadptr_t) thread;
    #endif
}
