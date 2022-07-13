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

#ifndef IOCOD_THREADS_H
#define IOCOD_THREADS_H

#include "iocod/platform.h"
#include "iocod/types.h"

typedef void *threadid_t;
typedef void *threadptr_t;

/**
 * @brief Get the current calling thread's ID.
 * @return the calling thread's ID
*/
IC_PUBLIC
threadid_t thread_current_thread_id(void);

/**
 * @brief Yield calling thread's execution to another thread.
*/
IC_PUBLIC
void thread_yield(void);

/**
 * @brief Set current thread priority to high.
*/
IC_PUBLIC
void thread_set_high_priority(void);

/**
 * @brief Exit the current thread with return code @p code.
 * @param[in] code exit code
 */
IC_PUBLIC
void thread_exit(int code);

IC_PUBLIC
/**
 * @brief Create a new thread.
 * @param[in] function   function to run in the new thread
 * @param[in] data       data to pass to the function
 * @param[in] name       name of the thread
 * @param[in] stack_size stack size of the thread, or 0 to let the OS decide
 * @return pointer to the thread instance, or NULL if failed
*/
threadptr_t thread_create(int (*function)(void *), void *data, const char *name,
                          size_t stack_size);

/**
 * @brief Destroy a given thread @p thread.
 * @param[in] thread thread to destroy
*/
IC_PUBLIC
void thread_destroy(threadptr_t thread);

/**
 * @brief Wait for thread @p thread to exit.
 * @param[in] thread thread to wait for
 * @return thread's exit code
*/
IC_PUBLIC
int thread_join(threadptr_t thread);

typedef union {
    void *align;
    char data[64];
} thread_mutex_t;

IC_PUBLIC
void thread_mutex_init(thread_mutex_t *mutex);

IC_PUBLIC
void thread_mutex_terminate(thread_mutex_t *mutex);

IC_PUBLIC
void thread_mutex_lock(thread_mutex_t *mutex);

IC_PUBLIC
void thread_mutex_unlock(thread_mutex_t *mutex);

typedef union {
    void *align;
    char data[116];
} thread_signal_t;

IC_PUBLIC
void thread_signal_init(thread_signal_t *signal);

IC_PUBLIC
void thread_signal_terminate(thread_signal_t *signal);

IC_PUBLIC
void thread_signal_raise(thread_signal_t *signal);

IC_PUBLIC
bool thread_signal_wait(thread_signal_t *signal, int timeout);

#endif /* IOCOD_THREADS_H */
