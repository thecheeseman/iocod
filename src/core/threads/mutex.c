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
void thread_mutex_init(thread_mutex_t *mutex)
{
    #ifdef IC_PLATFORM_WINDOWS
    // size check at compile-time
    #pragma warning(push)
    #pragma warning(disable:4214)
    struct x {
        char thread_mutex_type_too_small :
        (sizeof(thread_mutex_t) < sizeof(CRITICAL_SECTION) ? 0 : 1);
    };
    #pragma warning(pop)

    // on XP/2003 this can fail
    // Vista and newer will never fail
    InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION *) mutex, 32);
    #else
    struct x {
        char thread_mutex_type_too_small :
        (sizeof(thread_mutex_t) < sizeof(pthread_mutex_t) ? 0 : 1);
    };

    pthread_mutex_init((pthread_mutex_t *) mutex, NULL);
    #endif
}

IC_PUBLIC
void thread_mutex_terminate(thread_mutex_t *mutex)
{
    #ifdef IC_PLATFORM_WINDOWS
    DeleteCriticalSection((CRITICAL_SECTION *) mutex);
    #else
    pthread_mutex_destroy((pthread_mutex_t *) mutex);
    #endif
}

IC_PUBLIC
void thread_mutex_lock(thread_mutex_t *mutex)
{
    #ifdef IC_PLATFORM_WINDOWS
    EnterCriticalSection((CRITICAL_SECTION *) mutex);
    #else
    pthread_mutex_lock((pthread_mutex_t *) mutex);
    #endif
}

IC_PUBLIC
void thread_mutex_unlock(thread_mutex_t *mutex)
{
    #ifdef IC_PLATFORM_WINDOWS
    LeaveCriticalSection((CRITICAL_SECTION *) mutex);
    #else
    pthread_mutex_unlock((pthread_mutex_t *) mutex);
    #endif
}
