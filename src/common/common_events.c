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
 * @file common_events.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"

#define MAX_PUSHED_EVENTS 256
static int com_pushed_eventshead = 0;
static int com_pushed_eventstail = 0;
static struct system_event com_pushed_events[MAX_PUSHED_EVENTS];

void com_init_journaling(void)
{
	com_startup_variable("journal");

	com_journal = cvar_get("journal", "0", CVAR_INIT);
	if (com_journal->integer == 0)
		return;

	if (com_journal->integer == 1) {
		com_printf("Journaling events\n");

		com_journalfile = fs_fopen_file_write("journal.dat");
		com_journaldatafile = fs_fopen_file_write("journaldata.dat");
	} else if (com_journal->integer == 2) {
		com_printf("Replaying journaled events\n");

		fs_fopen_file_read("journal.dat", &com_journalfile, true, 0);
		fs_fopen_file_read("journaldata.dat", &com_journaldatafile, true, 0);
	}

	if (com_journalfile == 0 || com_journaldatafile == 0) {
		cvar_set("com_journal", "0");

		com_journalfile = 0;
		com_journaldatafile = 0;

		com_printf("Couldn't open journal files\n");
	}
}

void com_init_push_event(void)
{
	memset(com_pushed_events, 0, sizeof(com_pushed_events));

	com_pushed_eventshead = 0;
	com_pushed_eventstail = 0;
}

void com_push_event(struct system_event *event)
{
	struct system_event *ev;
	static bool printedwarning = false;

	ev = &com_pushed_events[com_pushed_eventshead & (MAX_PUSHED_EVENTS - 1)];

	if (com_pushed_eventshead - com_pushed_eventstail >= MAX_PUSHED_EVENTS) {
		if (!printedwarning) {
			printedwarning = true;
			com_printf("WARNING: com_push_event overflow\n");
		}

		if (ev->ptr != NULL)
			z_free(ev->ptr);

		com_pushed_eventstail++;
	} else {
		printedwarning = false;
	}

	*ev = *event;
	com_pushed_eventshead++;
}

struct system_event com_get_real_event(void)
{
	int r;
	struct system_event ev;

	// either get an event from the system or the journal file
	if (com_journal->integer == 2) {
		r = fs_read(&ev, sizeof(ev), com_journalfile);
		if (r != sizeof(ev))
			com_error(ERR_FATAL, "Error reading from journal file");

		if (ev.ptr_length > 0) {
			ev.ptr = z_malloc(ev.ptr_length);
			r = fs_read(ev.ptr, ev.ptr_length, com_journalfile);
			if (r != ev.ptr_length)
				com_error(ERR_FATAL, "Error reading from journal file");
		}
	} else {
		ev = sys_get_event();

		// write the journal value out if needed
		if (com_journal->integer == 1) {
			r = fs_write(&ev, sizeof(ev), com_journalfile);
			if (r != sizeof(ev))
				com_error(ERR_FATAL, "Error writing to journal file");

			if (ev.ptr_length > 0) {
				r = fs_write(ev.ptr, ev.ptr_length, com_journalfile);
				if (r != ev.ptr_length)
					com_error(ERR_FATAL, "Error writing to journal file");
			}
		}
	}

	return ev;
}

struct system_event com_get_event(void)
{
	if (com_pushed_eventshead > com_pushed_eventstail) {
		com_pushed_eventstail++;

		return com_pushed_events[(com_pushed_eventstail - 1) 
			& (MAX_PUSHED_EVENTS - 1)];
	}

	return com_get_real_event();
}

void com_run_and_time_server_packet(struct netadr *evfrom, struct msg *buf)
{
	int t1, t2, msec;

	t1 = 0;
	if (com_speeds->integer > 0)
		t1 = sys_milliseconds();

	sv_packet_event(*evfrom, buf);

	if (com_speeds->integer > 0) {
		t2 = sys_milliseconds();

		msec = t2 - t1;
		if (com_speeds->integer == 3)
			com_printf("sv_packet_event time: %i\n", msec);
	}
}

// event loop
int com_event_loop(void)
{
	struct system_event ev;
	struct netadr evfrom;
	byte bufdata[MAX_MSGLEN];
	struct msg buf;

	// msg_init
	msg_init(&buf, bufdata, sizeof(bufdata));

	while (true) {
		ev = com_get_event();

		switch (ev.type) {
		case SE_KEY:
			cl_key_event(ev.value, ev.value2, ev.time);
			break;
		case SE_CHAR:
			cl_char_event(ev.value);
			break;
		case SE_MOUSE:
			cl_mouse_event(ev.value, ev.value2, ev.time);
			break;
		case SE_JOYSTICK_AXIS:
			cl_joystick_event(ev.value, ev.value2, ev.time);
			break;
		case SE_CONSOLE:
			cbuf_add_text((char *) ev.ptr);
			cbuf_add_text("\n");
			break;
		case SE_PACKET:
			if ((unsigned) buf.cursize > (unsigned) buf.maxsize) {
				com_printf("com_event_loop: oversize packet\n");

				if (ev.ptr != NULL)
					z_free(ev.ptr);

				continue;
			}

			memcpy(buf.data, (byte *)((struct netadr *) ev.ptr + 1), 
					buf.cursize);

			if (ev.ptr != NULL)
				z_free(ev.ptr);

			if (com_sv_running->integer > 0)
				com_run_and_time_server_packet(&evfrom, &buf);
			else
				cl_packet_event(evfrom, &buf);
			break;
		case SE_BAD_EVENT:
			com_error(ERR_FATAL, "com_event_loop: bad event type %i", ev.type);
			break;
		default:
			// manually send packet events for the loopback channel
			while (net_get_loop_packet(NS_CLIENT, &evfrom, &buf))
				cl_packet_event(evfrom, &buf);

			while (net_get_loop_packet(NS_SERVER, &evfrom, &buf)) {
				if (com_sv_running->integer > 0)
					com_run_and_time_server_packet(&evfrom, &buf);
			}
			return ev.time;
		}

		if (ev.ptr != NULL)
			z_free(ev.ptr);
	}

	return 0;
}
