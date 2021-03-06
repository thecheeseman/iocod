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

#include "cvar/cvar.h"

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

void cl_console_print(UNUSED const char *msg)
{

}

void cl_disconnect(UNUSED bool showmainmenu)
{

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

void cl_key_event(UNUSED int value, UNUSED int value2, UNUSED int time)
{

}

void cl_char_event(UNUSED int value)
{

}

void cl_mouse_event(UNUSED int value, UNUSED int value2, UNUSED int time)
{

}

void cl_joystick_event(UNUSED int value, UNUSED int value2, UNUSED int time)
{

}

void cl_packet_event(UNUSED struct netadr evfrom, UNUSED struct msg *buf)
{

}

void cl_frame(UNUSED int msec, UNUSED int timescale)
{

}

void key_write_bindings(UNUSED filehandle f)
{

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

void cl_forward_command_to_server(UNUSED const char *text)
{

}
