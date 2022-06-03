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
    SE_NONE = 0,        /**< marks the end of the queue */

    SE_KEY,             /**< key up/down event
                             value = keycode
                             value2 = down flag */
    SE_CHAR,            /**< normal key chars (console, chatfields, etc) 
                             value = keycode */
    SE_MOUSE,           /**< mouse event 
                             value = relative signed x move
                             value2 = relative signed y move */
    SE_JOYSTICK_AXIS,   /**< joystick event 
                             value = axis number
                             value2 = current state (-127 to 128) */

    SE_CONSOLE,         /**< events from actual stdin/tty console 
                             ptr = `char *` */
    SE_PACKET,          /**< network packet events 
                             ptr = `struct netadr` followed by data bytes
                             ptr_length = total number of bytes */

    SE_BAD_EVENT        /**< should never happen */
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

/**
 * @brief Push an event into the pushed events loop.
 * @param event event to push
*/
IC_PUBLIC
void ev_push(struct sys_event *event);

/**
 * @brief Queue an event.
 * @param time event time, or 0 for current system time
 * @param type event type
 * @param value 
 * @param value2
 * @param ptr_length if ptr, length of ptr
 * @param ptr
*/
IC_PUBLIC
void ev_queue(int time, enum sys_event_type type, int value, int value2,
              int ptr_length, void *ptr);

/** @} */

#endif /* IC_SEVENTS_H */
