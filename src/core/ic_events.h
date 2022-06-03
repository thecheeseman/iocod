/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

/**
 * @defgroup events Events
 * @brief Event handling.
 * @{
 */

#ifndef IC_EVENTS_H
#define IC_EVENTS_H

enum sys_event_type {
    SE_NONE = 0,

    // client
    SE_KEY,
    SE_CHAR,
    SE_MOUSE,
    SE_JOYSTICK_AXIS,

    SE_CONSOLE,
    SE_PACKET,
    SE_BAD_EVENT
};

struct sys_event {
    int time;
    
    enum sys_event_type type;

    int value;
    int value2;

    int ptr_length;
    void *ptr;
};

#define MAX_MSG_LEN 16384

/**
 * @brief Initialize event queues.
*/
IC_PUBLIC
void ev_init(void);

/**
 * @brief Get the next real event.
 */
IC_PUBLIC
struct sys_event ev_get_real(void);

/**
 * @brief Get the next event.
*/
IC_PUBLIC
struct sys_event ev_get(void);

/**
 * @brief Main event loop.
 */
IC_PUBLIC
int ev_loop(void);

IC_PUBLIC
void ev_push(struct sys_event *event);

/**
 * @brief Queue an event.
 * @param time
 * @param type
 * @param value
 * @param value2
 * @param ptr_length
 * @param ptr
*/
IC_PUBLIC
void ev_queue(int time, enum sys_event_type type, int value, int value2,
              int ptr_length, void *ptr);

/** @} */

#endif /* IC_SEVENTS_H */
