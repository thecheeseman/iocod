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
    /**
     * @brief Marks the end of the system event queue.
    */
    SE_NONE = 0,

    /**
     * @brief Key up/down event.
     * 
     * @ref sys_event.value is the ASCII keycode of this key.
     * 
     * @ref sys_event.value2 is `1` if the key is down.
    */
    SE_KEY,

    /**
     * @brief Normal keyboard characters used for in-game console/chatfields,
     * etc.
     * 
     * @ref sys_event.value is the ASCII keycode of this key.
    */
    SE_CHAR,

    /**
     * @brief Mouse event.
     * 
     * @ref sys_event.value is the relative signed `x` coord. 
     * 
     * @ref sys_event.value2 is the relative signed `y` coord.
    */
    SE_MOUSE,

    /**
     * @brief Joystick event.
     * 
     * @ref sys_event.value is the axis number of the joystick move.
     * 
     * @ref sys_event.value2 is the current state (-127 to 128).
    */
    SE_JOYSTICK_AXIS,

    /**
     * @brief Event from stdin/tty console.
     * 
     * @ref sys_event.ptr is a pointer to the console string data.
    */
    SE_CONSOLE,

    /**
     * @brief Network packet event.
     * 
     * @ref sys_event.ptr is a pointer to a @ref netadr structure, followed by
     * the actual packet data.
     * 
     * @ref sys_event.ptr_length is the length of the packet in bytes.
    */
    SE_PACKET,

    /**
     * @brief An event type that should never occur.
    */
    SE_BAD_EVENT
};

/**
 * @brief System event structure.
*/
struct sys_event {
    /**
     * @brief Time of system event in milliseconds since startup.
    */
    int time;

    /**
     * @brief Type of event
     * @see sys_event_type
    */
    enum sys_event_type type;

    /**
     * @brief Depending on @ref type, this will hold a value with a different 
     * meaning.
     * 
     * For a given @ref type, this means:
     * @code
     * type                 meaning
     * ----------------------------------------------
     * SE_KEY               ASCII key code
     * SE_CHAR              ASCII key code
     * SE_MOUSE             relative signed `x` coord 
     * SE_JOYSTICK_AXIS     axis number 
     * @endcode
     * 
     * Otherwise, this is unused.
    */
    int value;

    /**
     * @brief Depending on @ref type, this will hold a value with a different
     * meaning.
     * 
     * For a given @ref type, this means:
     * @code
     * type                 meaning
     * ------------------------------------------------------
     * SE_KEY               1 if the key is down
     * SE_MOUSE             relative signed `y` coord
     * SE_JOYSTICK_AXIS     current axis state (-127 to +128)
     * @endcode
     * 
     * Otherwise, this is unused.
    */
    int value2;

    /**
     * @brief Length of @ref ptr. 
     * 
     * Only used if @ref type is @ref SE_PACKET.
    */
    int ptr_length;

    /**
     * @brief Pointer to data used for @ref SE_CONSOLE and @ref SE_PACKET 
     * event types.
     * 
     * If @ref type is @ref SE_CONSOLE, this will be a pointer to the console
     * string data. If @ref type is @ref SE_PACKET, this will be a pointer to 
     * a @ref netadr structure, followed directly by the actual packet data.
     * 
     * Otherwise, this is unused.
    */
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
 * @param[in] event event to push
*/
IC_PUBLIC
void ev_push(struct sys_event *event);

/**
 * @brief Queue an event.
 * @param[in] time          event time, or specify `0` for current system time
 * @param[in] type          type of event
 * @param[in] value         event value, if applicable
 * @param[in] value2        event value2, if applicable
 * @param[in] ptr_length    length of @p ptr, if applicable
 * @param[in] ptr           pointer to data, or NULL
 * 
 * @see sys_event
 * @see sys_event_type
*/
IC_PUBLIC
void ev_queue(int time, enum sys_event_type type, int value, int value2,
              int ptr_length, void *ptr);

/** @} */

#endif /* IC_EVENTS_H */
