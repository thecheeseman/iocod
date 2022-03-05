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
 * @file game_main.c
 * @date 2022-02-04
*/

#include "game_local.h"

// TEMP
struct bg {
	char data[12];
};

struct bg bg;

struct bgs {
	char data[706796];
};

struct bgs bgs;

int randomseed = 0x89ABCDEF;
vec3_t player_mins, player_maxs;

struct level level;

// made this static inline because why
static inline void rand_init(int seed)
{
	randomseed = seed;
}

/**
 * @brief 
 * @param  
*/
static void set_player_size(void)
{
	player_mins[0] = g_bounds_width.value * -0.5;
	player_mins[1] = g_bounds_width.value * -0.5;
	player_maxs[0] = g_bounds_width.value * 0.5;
	player_maxs[1] = g_bounds_width.value * 0.5;
	player_maxs[2] = g_bounds_height_standing.value;
}

static void setup_logging(void)
{
	char server_info[MAX_STRING_CHARS];

	if (g_log.string[0]) {
		if (g_log_sync.integer)
			trap_fs_fopen_file(g_log.string, &level.logfile, FS_APPEND_SYNC);
		else
			trap_fs_fopen_file(g_log.string, &level.logfile, FS_APPEND);

		if (!level.logfile) {
			g_printf("WARNING: couldn't open logfile '%s'\n", g_log.string);
		} else {
			trap_get_server_info(server_info, sizeof(server_info));

			g_log_printf("------------------------------------------------------------\n");
			g_log_printf("init_game: %s\n", server_info);
		}

	} else {
		g_printf("not logging to disk\n");
	}
}

extern void set_up_weapon_info(void);

/**
 * @brief 
 * @param level_time 
 * @param random_seed 
 * @param restart 
 * @param param4 
*/
void g_init_game(int level_time, int random_seed, int restart, int param4)
{
	char info_winner[MAX_STRING_CHARS];

	// swap_init
	g_printf("gamename: %s\n", BASEGAME);
	g_printf("gamedate: %s\n", __DATE__);

	srand(random_seed);
	rand_init(trap_milliseconds());

	scr_parse_gametype_list();

	// 1.1 only checks param4, 1.5 checks both
	if (!restart || !param4)
		register_cvars();

	process_ip_bans();
	set_player_size();

	// memsets
	memset(&level, 0, sizeof(level));
	memset(&bg, 0, sizeof(bg));
	memset(&bgs, 0, sizeof(bgs));

	level.initializing = true;
	level.time = level_time;
	level.start_time = level_time;

	#ifdef CODVERSION1_5
	// level.5956_4 = g_timeout_bank.integer;
	// level.5960_4 = g_timeout_bank.integer;

	// bgs.636624_4 = trap_com_soundalias_string;
	// bgs.636628_4 = g_anim_script_sound;
	#endif

	setup_logging();
	
	set_up_weapon_info();
	/*
	gscr_load_scripts();
	gscr_load_consts();
	*/
	trap_get_config_string(CS_WINNER, info_winner, sizeof(info_winner));
	info_set_value_for_key(info_winner, "winner", "0");
	trap_set_config_string(CS_WINNER, info_winner);
	/*
	memset(g_entities, 0, MAX_GENTITIES * sizeof(g_entities[0]));
	level.gentities = g_entities;

	level.max_clients = g_max_clients.integer;
	memset(g_clients, 0, MAX_CLIENTS * sizeof(g_clients[0]));
	level.clients = g_clients;

	level.num_entities = MAX_CLIENTS + 8;
	level.last_entity = NULL;
	level.next_entity = NULL;

	trap_locate_game_data(level.gentities, level.num_entities,
						  sizeof(struct gentity), &level.clients[0].ps,
						  sizeof(level.clients[0]));

	g_parse_hitloc_damage_table();
	*/

	if (!restart)
		clear_registered_items();

	/*
	g_init_turrets();
	g_spawn_entities_from_string();
	g_find_teams();
	save_registered_items();

	g_set_fog("0");

	g_init_objectives();
	*/
	g_printf("-----------------------------------\n");
	/*
	scr_init_system(true, level.time);
	scr_set_loading(true);
	scr_alloc_game_variable();
	scr_load_gametype();
	scr_load_level();
	scr_startup_gametype();
	*/

	model_index("xmodel/weapon_mg42");

	level.initializing = false;
}

void g_shutdown_game(bool restart)
{
	if (restart)
		g_printf("==== RestartGame ====\n");
	else
		g_printf("==== ShutdownGame ====\n");

	if (level.logfile) {
		if (restart)
			g_log_printf("RestartGame:\n");
		else
			g_log_printf("ShutdownGame:\n");

		g_log_printf("----------------------------------------------------");
		trap_fs_fclose_file(level.logfile);
	}

	// free entities

	// free entities
	// hudelem_destroy_all();

	trap_free_weapon_info_memory(0); // not sure what the 0 is for but whatever
}

void g_run_frame(int time)
{
	level.frame_num++;
	level.previous_time = level.time;
	level.time = time;
	level.frame_time = time - level.previous_time;

	update_cvars();
}

extern intptr_t scr_far_hook(intptr_t addr);

int DLLEXPORT vmMain(enum game_exports command, int arg0, int arg1, int arg2,
					 int arg3, int arg4)
{
	switch (command) {
		case GAME_INIT:
			g_init_game(arg0, arg1, arg2, arg3);
			break;

		#ifdef CODVERSION1_5
		case GAME_VERSION:
			return 5;
		case GAME_IDK:
			break;
		#endif

		case GAME_SHUTDOWN:
			g_shutdown_game(arg0);
			break;

		case GAME_CLIENT_CONNECT:
		case GAME_CLIENT_BEGIN:
		case GAME_CLIENT_USERINFO_CHANGED:
		case GAME_CLIENT_DISCONNECT:
		case GAME_CLIENT_COMMAND:
		case GAME_CLIENT_THINK:
		case GAME_GET_FOLLOW_PLAYERSTATE:
		case GAME_UPDATE_CVARS:
		case GAME_RUN_FRAME:
			g_run_frame(arg0);
			break;
		case GAME_CONSOLE_COMMAND:
			return console_command();
		case GAME_SCRIPT_FARHOOK:
			return (intptr_t) scr_far_hook(arg0);
			break;

		case GAME_CALC_POSE:
		case GAME_VALID_WEAPON:
		case GAME_SET_PERSISTENT:
		case GAME_GET_PERSISTENT:
		case GAME_GET_CLIENT_2178:
		case GAME_GET_CLIENT_ARCHIVETIME:
		case GAME_SET_CLIENT_ARCHIVETIME:
		case GAME_GET_CLIENT_SCORE:
			break;

		default:
			trap_printf(va("unknown systemcall %d\n", command));
			break;
	}

	return 0;
}
