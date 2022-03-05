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
 * @file game_local.h
 * @date 2022-02-04
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "shared.h"
#include "game.h"
#include "global/cvar.h"
#include "script/script_local.h"
#include "bg/bg_local.h"

extern vec3_t player_mins, player_maxs;
extern int randomseed;

// TEMP
struct gentity {
	int a;
};
// TEMP

#define DEFAULT_GRAVITY			800

#define MAX_ITEMS				256

#define MAX_OBJECTIVES			16

#define CS_GAME_VERSION			2
#define CS_AMBIENT_PLAY			3
#define CS_MESSAGE				4
#define CS_SCORES1				5
#define CS_SCORES2				6
#define CS_WEAPONS				7
#define CS_ITEMS				8
#define CS_NORTHYAW				11
#define CS_FOGVARS				12
#define CS_MOTD					14
#define CS_VOTE_TIME			15
#define CS_VOTE_STRING			16
#define CS_VOTE_YES				17
#define CS_VOTE_NO				18
#define CS_WINNER				20
#define CS_STATUS_ICONS			21
#define CS_HEAD_ICONS			(CS_STATUS_ICONS + MAX_STATUS_ICONS)
#define CS_LOCATIONS			(CS_HEAD_ICONS + MAX_HEAD_ICONS)
#define CS_TAG_CONNECTS			(CS_LOCATIONS + MAX_LOCATIONS)
#define CS_CLIENT_VAR_KEYS		(CS_TAG_CONNECTS + MAX_TAG_CONNECTS)
#define CS_CLIENT_VAR_VALUES	(CS_CLIENT_VAR_KEYS + MAX_CLIENT_VARS)
#define CS_MODELS				(CS_CLIENT_VAR_VALUES + MAX_CLIENT_VARS)
#define CS_SOUNDS				(CS_MODELS + MAX_MODELS)
#define CS_FX					(CS_SOUNDS + MAX_SOUNDS)
#define CS_FX_ON_TAG			(CS_FX + MAX_FX)
#define CS_SHELLSHOCKS			(CS_FX_ON_TAG + MAX_FX_ON_TAG)

#define CS_MENUS				1180
#define CS_HINT_STRINGS			(CS_MENUS + MAX_MENUS)
#define CS_LOCALIZED_STRINGS	(CS_HINT_STRINGS + MAX_HINT_STRINGS)
#define CS_SHADERS				(CS_LOCALIZED_STRINGS + MAX_LOCALIZED_STRINGS)

//
// gentity->flags
//
#define FL_GOD_MODE		0x01
#define FL_NO_TARGET	0x02

struct objective {
	int state;
	vec3_t origin;
	int entity_number;
	int team;
	int shader_index;
};

//
// level structure
//
#define MAX_SPAWN_VARS          64
#define MAX_SPAWN_VAR_CHARS		2048

struct level {
	void *clients;
	void *gentities;
	int gentity_size;
	int num_entities;
	void **last_entity;
	void **next_entity;

	filehandle logfile;

	bool initializing;

	struct objective objectives[MAX_OBJECTIVES];

	int max_clients;

	int frame_num;

	int time;
	int previous_time;
	int frame_time;
	int start_time;

	int team_scores[4];

	int last_team_location_time;
	int intermission_time;

	int client_name_mode;
	int num_connected_clients;
	int sorted_clients[MAX_CLIENTS];

	char vote_string[MAX_STRING_CHARS];
	char vote_display_string[MAX_STRING_CHARS];
	int vote_time;
	int vote_execute_time;
	int vote_yes;
	int vote_no;
	int num_voting_clients;

	
	char team_vote_string[MAX_STRING_CHARS][2];
	int team_vote_time[2];
	int team_vote_yes[2];
	int team_vote_no[2];

	bool spawning;
	int num_spawn_vars;
	char *spawn_vars[MAX_SPAWN_VARS][2];
	int num_spawn_var_chars;
	char spawn_var_chars[MAX_SPAWN_VAR_CHARS];
	
	bool keep_persistent;
	
	bool location_linked;
	void *location_head;
	
	char data2[128];

	float fog_density;
	float fog_density_squared;

	int field_1de4;

	int body_queue_index;

	char data3[3084];

	int radius_damage_something;
	bool ignore_radius_damage;
	float bounds_width;
	float bounds_height;
	float viewheight_standing;
	float viewheight_crouched;
	float viewheight_proned;

	int field_10768;
};

extern struct level level;

//
//
//
enum item_type {
	IT_BAD,
	IT_WEAPON,
	IT_AMMO,
	IT_HEALTH
};

struct gitem {
	char *class_name;
	char *pickup_sound;
	char *world_model[2];
	char *hud_icon;
	char *ammo_icon;
	char *display_name;

	int quantity;

	int type;
	int tag;

	int ammo_name_index;
	int clip_name_index;
};

extern struct gitem bg_item_list[];
extern int bg_num_items;


//
// hudelems
//
struct hudelem {
	int type;

	int x;
	int y;

	float fontscale; 
	int font;

	int align_x;
	int align_y;

	byte color[4];

	byte old_color[4];
	int fade_over_time_start; 
	int fade_over_time_length; 

	int label;

	int shader_x;
	int shader_y;
	int shader;

	int old_shader_x;
	int old_shader_y;
	int scale_over_time_start;
	int scale_over_time_length;
	int old_start_x;
	int old_start_y;
	int move_over_time_start; 
	int move_over_time_length;

	int clock_start;
	int clock_division;

	float value;
	int string;

	float sort;

	int entity_number;
	int team;
	bool archived;
};

extern struct hudelem g_hudelems[];

#define MAX_GAMETYPES 32

struct gametype {
	char *name;
	char *description;
	bool team_based;	// unused really
};

struct game_script_data {
	int map_main;

	int field_4;

	int gametype_main;
	int start_gametype;
	int player_connect;
	int player_disconnect;
	int player_damage;
	int player_killed;

	int num_gametypes;

	struct gametype gametypes[MAX_GAMETYPES];
};

extern struct game_script_data g_scr_data;

//
// game cvars
//
extern struct vmcvar g_cheats;
extern struct vmcvar g_gametype;
extern struct vmcvar g_max_clients;
extern struct vmcvar g_synchronous_clients;
extern struct vmcvar g_intermission_delay;
extern struct vmcvar g_log;
extern struct vmcvar g_log_sync;
extern struct vmcvar g_password;
extern struct vmcvar g_ban_ips;
extern struct vmcvar g_dedicated;
extern struct vmcvar g_speed;
extern struct vmcvar g_gravity;
extern struct vmcvar g_knockback;
extern struct vmcvar g_weapon_respawn;
extern struct vmcvar g_weapon_ammo_pools;

#ifdef CODVERSION1_5
extern struct vmcvar g_max_dropped_weapons;
#endif

extern struct vmcvar g_inactivity;
extern struct vmcvar g_debug_move;
extern struct vmcvar g_debug_prone_check;
extern struct vmcvar g_debug_prone_check_depth_check;
extern struct vmcvar g_debug_damage;
extern struct vmcvar g_debug_alloc;
extern struct vmcvar g_debug_bullets;
extern struct vmcvar g_motd;
extern struct vmcvar g_allow_vote;

#ifdef CODVERSION1_5
extern struct vmcvar g_allow_vote_map_restart;
extern struct vmcvar g_allow_vote_map_rotate;
extern struct vmcvar g_allow_vote_type_map;
extern struct vmcvar g_allow_vote_map;
extern struct vmcvar g_allow_vote_gametype;
extern struct vmcvar g_allow_vote_kick;
extern struct vmcvar g_allow_vote_client_kick;
extern struct vmcvar g_allow_vote_temp_ban_user;
extern struct vmcvar g_allow_vote_temp_ban_client;
extern struct vmcvar g_allow_vote_draw_friend;
extern struct vmcvar g_allow_vote_killcam;
extern struct vmcvar g_allow_vote_friendly_fire;
#endif

extern struct vmcvar g_list_entity;

#ifdef CODVERSION1_5
extern struct vmcvar ui_allow_vote;
extern struct vmcvar ui_allow_vote_map_restart;
extern struct vmcvar ui_allow_vote_map_rotate;
extern struct vmcvar ui_allow_vote_type_map;
extern struct vmcvar ui_allow_vote_map;
extern struct vmcvar ui_allow_vote_gametype;
extern struct vmcvar ui_allow_vote_kick;
extern struct vmcvar ui_allow_vote_client_kick;
extern struct vmcvar ui_allow_vote_temp_ban_user;
extern struct vmcvar ui_allow_vote_temp_ban_client;
extern struct vmcvar ui_allow_vote_draw_friend;
extern struct vmcvar ui_allow_vote_killcam;
extern struct vmcvar ui_allow_vote_friendly_fire;
extern struct vmcvar scr_draw_friend;
extern struct vmcvar scr_friendly_fire;
extern struct vmcvar scr_killcam;
extern struct vmcvar ui_draw_friend;
extern struct vmcvar ui_friendly_fire;
extern struct vmcvar ui_killcam;
#endif

extern struct vmcvar g_complaint_limit;
extern struct vmcvar g_voice_chats_allowed;

#ifdef CODVERSION1_5
extern struct vmcvar g_dead_chat;
#endif

extern struct vmcvar g_developer;
extern struct vmcvar g_scores_banner_allies;
extern struct vmcvar g_scores_banner_axis;
extern struct vmcvar g_scores_banner_none;
extern struct vmcvar g_scores_banner_spectators;
extern struct vmcvar g_team_name_allies;
extern struct vmcvar g_team_name_axis;
extern struct vmcvar g_team_color_allies;
extern struct vmcvar g_team_color_axis;
extern struct vmcvar g_smoothclients;
extern struct vmcvar pmove_fixed;
extern struct vmcvar pmove_msec;
extern struct vmcvar g_script_main_menu;
extern struct vmcvar bg_view_height_standing;
extern struct vmcvar bg_view_height_crouched;
extern struct vmcvar bg_view_height_prone;

// these are only present in 1.1
#ifndef CODVERSION1_5
extern struct vmcvar bg_duck2prone_time;
extern struct vmcvar bg_prone2duck_time;
#endif

extern struct vmcvar bg_ladder_yaw_cap;
extern struct vmcvar bg_prone_yaw_cap;

#ifndef CODVERSION1_5
extern struct vmcvar bg_prone_soft_yaw_edge;
#endif

extern struct vmcvar bg_foliagesnd_min_speed;
extern struct vmcvar bg_foliagesnd_max_speed;
extern struct vmcvar bg_foliagesnd_slow_interval;
extern struct vmcvar bg_foliagesnd_fast_interval;
extern struct vmcvar bg_foliagesnd_reset_interval;
extern struct vmcvar bg_fall_damage_min_height;
extern struct vmcvar bg_fall_damage_max_height;
extern struct vmcvar bg_debug_weapon_anim;
extern struct vmcvar bg_debug_weapon_state;

#ifdef CODVERSION1_5
extern struct vmcvar cg_bob_amplitude_standing;
extern struct vmcvar cg_bob_amplitude_ducked;
extern struct vmcvar cg_bob_amplitude_prone;
extern struct vmcvar cg_bob_max;
#endif

extern struct vmcvar g_bounds_width;
extern struct vmcvar g_bounds_height_standing;
extern struct vmcvar g_no_script_spam;
extern struct vmcvar g_debug_show_hit;
extern struct vmcvar g_debug_loc_damage;
extern struct vmcvar bg_debug_anim;
extern struct vmcvar bg_swing_speed;
extern struct vmcvar g_use_gear;
extern struct vmcvar g_language_warnings;
extern struct vmcvar g_language_warnings_as_errors;
extern struct vmcvar g_dump_anims;

#ifdef CODVERSION1_5
extern struct vmcvar g_auto_screenshot;
extern struct vmcvar g_auto_demo;
extern struct vmcvar g_timeouts_allowed;
extern struct vmcvar g_timeout_bank;
extern struct vmcvar g_timeout_length;
extern struct vmcvar g_timeout_recovery;
extern struct vmcvar ui_timeouts_allowed;
extern struct vmcvar ui_timeout_bank;
extern struct vmcvar ui_timeout_length;
extern struct vmcvar ui_timeout_recovery;
#endif

//
//
// game_cmds.c
//
//
char *concat_args(int start);

//
//
// game_common.c
//
//
void g_printf(const char *fmt, ...);
void g_dprintf(const char *fmt, ...);
void g_error(const char *fmt, ...);
void com_printf_runner(enum print_level level, const char *fmt, ...);
void com_error_runner(enum error_code code, const char *file, const char *func,
					  int line, const char *fmt, ...);
void g_log_printf(const char *fmt, ...);

//
//
// game_cvars.c
//
//
void register_cvars(void);
void update_cvars(void);

//
//
// game_items.c
//
//
void clear_registered_items(void);
void register_item(int num, int other);

//
//
// game_misc.c
//
//
bool get_hint_string_index(int *hint, char *string);
int model_index(const char *model);

//
//
// game_svcmds.c
//
//
void process_ip_bans(void);
bool console_command(void);

//
//
// game_syscalls.c
//
//
void trap_printf(const char *fmt);
void trap_error(const char *fmt);
void trap_error_localized(const char *fmt);
int trap_milliseconds(void);
void trap_cvar_register(struct vmcvar *cvar, const char *var_name,
						const char *value, int flags);
void trap_cvar_update(struct vmcvar *cvar);
void trap_cvar_set(const char *var_name, const char *value);
int trap_cvar_variable_integer_value(const char *var_name);
float trap_cvar_variable_value(const char *var_name);
void trap_cvar_variable_string_buffer(const char *var_name, char *buffer,
									  int size);
int trap_argc(void);
void trap_argv(int n, char *buffer, int buffer_length);
void *trap_hunk_alloc_internal(int size);
void *trap_hunk_alloc_low_internal(int size);
void *trap_hunk_alloc_align_internal(int size, int align);
void *trap_hunk_alloc_low_align_internal(int size, int align);
void *trap_hunk_allocate_temp_memory_internal(int size);
void trap_hunk_free_temp_memory_internal(int size);
int trap_fs_fopen_file(const char *qpath, filehandle *f, enum fs_mode mode);
void trap_fs_read(void *buffer, int len, filehandle f);
int trap_fs_write(const void *buffer, int len, filehandle f);
int trap_fs_rename(const char *from, const char *to);
void trap_fs_fclose_file(filehandle file);
void trap_send_console_command(int exec_when, const char *text);
void trap_locate_game_data(struct gentity *gents, int num_gentities,
						   int gentity_size, struct player_state *clients,
						   int client_size);
void trap_drop_client(int client_num, const char *reason);
void trap_send_server_command(int client, int a, const char *text);
void trap_set_config_string(int num, const char *string);
void trap_get_config_string(int num, char *buffer, int size);
const char *trap_get_config_string_const(int num);
void trap_get_server_info(char *buffer, int size);
int trap_fs_get_file_list(const char *path, const char *extension,
						   char *buffer, int size);
struct weapon **trap_get_weapon_info_memory(int size, int *already_allocated);
void trap_free_weapon_info_memory(int a);

#endif // __GAME_H__
