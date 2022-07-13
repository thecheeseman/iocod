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
int thread_join(threadptr_t thread)
{
    #ifdef IC_PLATFORM_WINDOWS
    WaitForSingleObject((HANDLE) thread, INFINITE);
    DWORD ret;
    GetExitCodeThread((HANDLE) thread, &ret);
    return (int) ret;
    #else
    void *ret;
    pthread_join((pthread_t) thread, &ret);
    return (int) ret;
    #endif
}
