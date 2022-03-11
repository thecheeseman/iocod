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
 * @file null_client.c
 * @date 2022-02-04
 * 
 * All the functions here are blank templates in the event that
 * this project ends up as a full replacement for the client and server
*/

#include "shared.h"
#include "common.h"

struct cvar *cl_shownet;
struct cvar *cl_language;

void cl_init(void)
{
	cl_shownet = cvar_get("cl_shownet", "0", CVAR_TEMP);
	cl_language = cvar_get("cl_language", "0", CVAR_ARCHIVE);
}

void cl_init_key_commands(void)
{

}

void cl_start_hunk_users(void)
{

}

void cl_console_print(const char *msg)
{
	UNUSED(msg);
}

void cl_disconnect(bool showmainmenu)
{
	UNUSED(showmainmenu);
}

void cl_flush_memory(void)
{

}

void cl_shutdown(void)
{

}

void cl_cd_dialog(void)
{

}

void s_clear_sound_buffer(void)
{

}

void cl_key_event(int value, int value2, int time)
{
	UNUSED(value);
	UNUSED(value2);
	UNUSED(time);
}

void cl_char_event(int value)
{
	UNUSED(value);
}

void cl_mouse_event(int value, int value2, int time)
{
	UNUSED(value);
	UNUSED(value2);
	UNUSED(time);
}

void cl_joystick_event(int value, int value2, int time)
{
	UNUSED(value);
	UNUSED(value2);
	UNUSED(time);
}

void cl_packet_event(struct netadr evfrom, struct msg *buf)
{
	UNUSED(evfrom);
	UNUSED(buf);
}

void cl_frame(int msec, int timescale)
{
	UNUSED(msec);
	UNUSED(timescale);
}

void key_write_bindings(filehandle f)
{
	UNUSED(f);
}

void cl_map_loading(void)
{

}

void cl_shutdown_all(void)
{

}

bool cl_game_command(void)
{
	return false;
}

bool ui_game_command(void)
{
	return false;
}

void cl_forward_command_to_server(const char *text)
{
	UNUSED(text);
}
