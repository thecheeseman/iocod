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
 * @file unix_events.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"

#define MAX_QUEUED_EVENTS   256
#define MASK_QUEUED_EVENTS  (MAX_QUEUED_EVENTS - 1)

struct system_event eventqueue[MAX_QUEUED_EVENTS];
int eventhead = 0;
int eventtail = 0;
byte sys_packetreceived[MAX_MSGLEN];

void sys_events_init(void)
{
	memset(&eventqueue[0], 0, MAX_QUEUED_EVENTS * sizeof(struct system_event));
	memset(&sys_packetreceived[0], 0, MAX_MSGLEN * sizeof(byte));
}

void sys_queue_event(int time, enum system_event_type type, 
					 int value, int value2, int ptr_length, void *ptr)
{
	struct system_event *ev;

	ev = &eventqueue[eventhead & MASK_QUEUED_EVENTS];

	if (eventhead - eventtail >= MASK_QUEUED_EVENTS) {
		com_printf("sys_queue_event: overflow\n");

		if (ev->ptr)
			z_free(ev->ptr);

		eventtail++;
	}

	eventhead++;

	if (time == 0)
		time = sys_milliseconds();

	ev->time = time;
	ev->type = type;
	ev->value = value;
	ev->value2 = value2;
	ev->ptr_length = ptr_length;
	ev->ptr = ptr;
}

struct system_event sys_get_event(void)
{
	struct system_event ev;
	char *s = NULL;
	struct msg netmsg;
	struct netadr adr;

	// return if we have event data
	if (eventhead > eventtail) {
		eventtail++;
		return eventqueue[(eventtail - 1) & MASK_QUEUED_EVENTS];
	}

	// sys_send_key_events();

	// check for console commands
	s = sys_console_input();
	if (s) {
		char *b;
		int len;

		len = strlen(s) + 1;
		b = z_malloc(len);
		strcpy(b, s);
		sys_queue_event(0, SE_CONSOLE, 0, 0, len, b);
	}

	// in_frame();

	// check for network packets

	// return if we have event data
	if (eventhead > eventtail) {
		eventtail++;
		return eventqueue[(eventtail - 1) & MASK_QUEUED_EVENTS];
	}

	// create an empty event to return
	memset(&ev, 0, sizeof(ev));
	ev.time = sys_milliseconds();

	return ev;
}
