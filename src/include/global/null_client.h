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
 * @file null_client.h
 * @date 2022-02-04
*/

#ifndef __NULL_CLIENT_H__
#define __NULL_CLIENT_H__

#include "types/bool.h"

// empty client nonsense
void cl_init(void);
void cl_init_key_commands(void);
void cl_start_hunk_users(void);
void cl_console_print(const char *msg);
void cl_disconnect(bool showmainmenu);
void cl_flush_memory(void);
void cl_shutdown(void);
void cl_cd_dialog(void);
void s_clear_sound_buffer(void);
void cl_key_event(int value, int value2, int time);
void cl_char_event(int value);
void cl_mouse_event(int value, int value2, int time);
void cl_joystick_event(int value, int value2, int time);
void cl_frame(int msec, int timescale);
void key_write_bindings(filehandle f);

void cl_packet_event(struct netadr evfrom, struct msg *buf);

void cl_map_loading(void);
void cl_shutdown_all(void);

bool cl_game_command(void);
bool ui_game_command(void);
void cl_forward_command_to_server(const char *text);

#endif // __NULL_CLIENT_H__
