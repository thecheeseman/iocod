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

struct thread_internal_signal_t {
    #ifdef IC_PLATFORM_WINDOWS
    CRITICAL_SECTION mutex;
    CONDITION_VARIABLE condition;
    int value;
    #else
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int value;
    #endif
};

IC_PUBLIC
void thread_signal_init(thread_signal_t *signal)
{
    #pragma warning(push)
    #pragma warning(disable:4214)
    struct x {
        char thread_signal_type_too_small :
        (sizeof(thread_signal_t) < sizeof(struct thread_internal_signal_t) ? 0 : 1);
    };
    #pragma warning(pop)

    struct thread_internal_signal_t *_internal =
        (struct thread_internal_signal_t *) signal;

    #ifdef IC_PLATFORM_WINDOWS
    InitializeCriticalSectionAndSpinCount(&_internal->mutex, 32);
    InitializeConditionVariable(&_internal->condition);
    _internal->value = 0;
    #else
    pthread_mutex_init(&_internal->mutex, NULL);
    pthread_cond_init(&_internal->condition, NULL);
    _internal->value = 0;
    #endif
}

IC_PUBLIC
void thread_signal_terminate(thread_signal_t *signal)
{
    struct thread_internal_signal_t *_internal =
        (struct thread_internal_signal_t *) signal;

    #ifdef IC_PLATFORM_WINDOWS
    DeleteCriticalSection(&_internal->mutex);
    #else
    pthread_mutex_destroy(&_internal->mutex);
    pthread_cond_destroy(&_internal->condition);
    #endif
}

IC_PUBLIC
void thread_signal_raise(thread_signal_t *signal)
{
    struct thread_internal_signal_t *_internal =
        (struct thread_internal_signal_t *) signal;

    #ifdef IC_PLATFORM_WINDOWS
    EnterCriticalSection(&_internal->mutex);
    _internal->value = 1;
    LeaveCriticalSection(&_internal->mutex);
    WakeConditionVariable(&_internal->condition);
    #else
    pthread_mutex_lock(&_internal->mutex);
    _internal->value = 1;
    pthread_mutex_unlock(&_internal->mutex);
    pthread_cond_signal(&_internal->condition);
    #endif
}

IC_PUBLIC
bool thread_signal_wait(thread_signal_t *signal, int timeout)
{
    struct thread_internal_signal_t *_internal = 
        (struct thread_internal_signal_t *) signal;

    #ifdef IC_PLATFORM_WINDOWS
    bool timedout = false;

    EnterCriticalSection(&_internal->mutex);

    while (_internal->value == 0) {
        bool res = SleepConditionVariableCS(&_internal->condition, 
                                            &_internal->mutex, 
                                            timeout < 0 ? INFINITE : timeout);
        
        if (!res && GetLastError() == ERROR_TIMEOUT) { 
            timedout = true; 
            break; 
        }
    }
    
    if (!timedout) 
        _internal->value = 0;

    LeaveCriticalSection(&_internal->mutex);
    return !timedout;
    #else
    struct timespec ts = { 0 };
    if (timeout >= 0) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        ts.tv_sec = time(NULL) + timeout / 1000;
        ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (timeout % 1000);
        ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
        ts.tv_nsec %= (1000 * 1000 * 1000);
    }

    bool timedout = false;
    pthread_mutex_lock(&_internal->mutex);
    while (_internal->value == 0) {
        if (timeout < 0) {
            pthread_cond_wait(&_internal->condition, &_internal->mutex);
        } else if (pthread_cond_timedwait(&_internal->condition, 
                                          &_internal->mutex, &ts) == ETIMEDOUT) {
            timedout = true;
            break;
        }

    }

    if (!timedout) 
        _internal->value = 0;

    pthread_mutex_unlock(&_internal->mutex);
    return !timedout;
    #endif
}
