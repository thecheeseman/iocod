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
 * @file server.h
 * @date 2022-02-04
*/

#ifndef __SERVER_H__
#define __SERVER_H__

//
//
// server/sv_cmds.c
//
//

/**
 * @brief Remove any references to cmd data.
 * Should only be called on true shutdown.
*/
void sv_remove_operator_commands(void);

//
//
// server/sv_game.c
//
//
void sv_init_game_progs(bool keep_persistent);
void sv_shutdown_game_progs(void);
bool sv_game_command(void);

//
//
// server/sv_init.c
//
//
void sv_shutdown(char *finalmsg);
void sv_shutdown_localized(char *localizedmsg);
void sv_init(void);
void sv_spawn_server(char *server);
void sv_packet_event(struct netadr from, struct msg *msg);

//
//
// server/sv_main.c
//
//
void sv_frame(int msec);

#endif // __SERVER_H__
