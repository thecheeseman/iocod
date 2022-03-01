#include "game_local.h"

struct vmcvar g_cheats;
struct vmcvar g_gametype;
struct vmcvar g_max_clients;
struct vmcvar g_synchronous_clients;
struct vmcvar g_intermission_delay;
struct vmcvar g_log;
struct vmcvar g_log_sync;
struct vmcvar g_password;
struct vmcvar g_ban_ips;
struct vmcvar g_dedicated;
struct vmcvar g_speed;
struct vmcvar g_gravity;
struct vmcvar g_knockback;
struct vmcvar g_weapon_respawn;
struct vmcvar g_weapon_ammo_pools;

#ifdef CODVERSION1_5
struct vmcvar g_max_dropped_weapons;
#endif

struct vmcvar g_inactivity;
struct vmcvar g_debug_move;
struct vmcvar g_debug_prone_check;
struct vmcvar g_debug_prone_check_depth_check;
struct vmcvar g_debug_damage;
struct vmcvar g_debug_alloc;
struct vmcvar g_debug_bullets;
struct vmcvar g_motd;
struct vmcvar g_allow_vote;

#ifdef CODVERSION1_5
struct vmcvar g_allow_vote_map_restart;
struct vmcvar g_allow_vote_map_rotate;
struct vmcvar g_allow_vote_type_map;
struct vmcvar g_allow_vote_map;
struct vmcvar g_allow_vote_gametype;
struct vmcvar g_allow_vote_kick;
struct vmcvar g_allow_vote_client_kick;
struct vmcvar g_allow_vote_temp_ban_user;
struct vmcvar g_allow_vote_temp_ban_client;
struct vmcvar g_allow_vote_draw_friend;
struct vmcvar g_allow_vote_killcam;
struct vmcvar g_allow_vote_friendly_fire;
#endif

struct vmcvar g_list_entity;

#ifdef CODVERSION1_5
struct vmcvar ui_allow_vote;
struct vmcvar ui_allow_vote_map_restart;
struct vmcvar ui_allow_vote_map_rotate;
struct vmcvar ui_allow_vote_type_map;
struct vmcvar ui_allow_vote_map;
struct vmcvar ui_allow_vote_gametype;
struct vmcvar ui_allow_vote_kick;
struct vmcvar ui_allow_vote_client_kick;
struct vmcvar ui_allow_vote_temp_ban_user;
struct vmcvar ui_allow_vote_temp_ban_client;
struct vmcvar ui_allow_vote_draw_friend;
struct vmcvar ui_allow_vote_killcam;
struct vmcvar ui_allow_vote_friendly_fire;
struct vmcvar scr_draw_friend;
struct vmcvar scr_friendly_fire;
struct vmcvar scr_killcam;
struct vmcvar ui_draw_friend;
struct vmcvar ui_friendly_fire;
struct vmcvar ui_killcam;
#endif

struct vmcvar g_complaint_limit;
struct vmcvar g_voice_chats_allowed;

#ifdef CODVERSION1_5
struct vmcvar g_dead_chat;
#endif

struct vmcvar g_developer;
struct vmcvar g_scores_banner_allies;
struct vmcvar g_scores_banner_axis;
struct vmcvar g_scores_banner_none;
struct vmcvar g_scores_banner_spectators;
struct vmcvar g_team_name_allies;
struct vmcvar g_team_name_axis;
struct vmcvar g_team_color_allies;
struct vmcvar g_team_color_axis;
struct vmcvar g_smoothclients;
struct vmcvar pmove_fixed;
struct vmcvar pmove_msec;
struct vmcvar g_script_main_menu;
struct vmcvar bg_view_height_standing;
struct vmcvar bg_view_height_crouched;
struct vmcvar bg_view_height_prone;

// these are only present in 1.1
#ifndef CODVERSION1_5
struct vmcvar bg_duck2prone_time;
struct vmcvar bg_prone2duck_time;
#endif

struct vmcvar bg_ladder_yaw_cap;
struct vmcvar bg_prone_yaw_cap;

#ifndef CODVERSION1_5
struct vmcvar bg_prone_soft_yaw_edge;
#endif

struct vmcvar bg_foliagesnd_min_speed;
struct vmcvar bg_foliagesnd_max_speed;
struct vmcvar bg_foliagesnd_slow_interval;
struct vmcvar bg_foliagesnd_fast_interval;
struct vmcvar bg_foliagesnd_reset_interval;
struct vmcvar bg_fall_damage_min_height;
struct vmcvar bg_fall_damage_max_height;
struct vmcvar bg_debug_weapon_anim;
struct vmcvar bg_debug_weapon_state;

#ifdef CODVERSION1_5
struct vmcvar cg_bob_amplitude_standing;
struct vmcvar cg_bob_amplitude_ducked;
struct vmcvar cg_bob_amplitude_prone;
struct vmcvar cg_bob_max;
#endif

struct vmcvar g_bounds_width;
struct vmcvar g_bounds_height_standing;
struct vmcvar g_no_script_spam;
struct vmcvar g_debug_show_hit;
struct vmcvar g_debug_loc_damage;
struct vmcvar bg_debug_anim;
struct vmcvar bg_swing_speed;
struct vmcvar g_use_gear;
struct vmcvar g_language_warnings;
struct vmcvar g_language_warnings_as_errors;
struct vmcvar g_dump_anims;

#ifdef CODVERSION1_5
struct vmcvar g_auto_screenshot;
struct vmcvar g_auto_demo;
struct vmcvar g_timeouts_allowed;
struct vmcvar g_timeout_bank;
struct vmcvar g_timeout_length;
struct vmcvar g_timeout_recovery;
struct vmcvar ui_timeouts_allowed;
struct vmcvar ui_timeout_bank;
struct vmcvar ui_timeout_length;
struct vmcvar ui_timeout_recovery;
#endif

// cvar table defined here since it's long
#include "game_cvar_table.h"

void g_register_cvars(void)
{
	int i;
	struct cvar_table *cv;

	for (i = 0, cv = game_cvar_table; i < game_cvar_table_size; i++, cv++) {
		trap_cvar_register(cv->vm_cvar,
						   cv->cvar_name,
						   cv->default_string,
						   cv->cvar_flags);

		if (cv->vm_cvar)
			cv->modification_count = cv->vm_cvar->modification_count;
	}
	/*
	if (!scr_is_valid_gametype(g_gametype.string)) {
		g_printf("g_gametype %s is not a valid gametype, defaulting to dm\n",
				 g_gametype.string);
		trap_cvar_set("g_gametype", "dm");
		trap_cvar_update(&g_gametype);
	}*/
}