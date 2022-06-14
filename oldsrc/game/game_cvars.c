#include "game_local.h"
#include "va.h"

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

//
//
//
//
//
struct cvar_table {
    struct vmcvar *vm_cvar;
    char *cvar_name;
    char *default_string;
    int cvar_flags;
    int modification_count;
    bool track_change;
};

static struct cvar_table cvar_table[] = {
    #define BLANK(n, d, f) \
        { \
            .vm_cvar = NULL, \
            .cvar_name = n, \
            .default_string = d, \
            .cvar_flags = f, \
            .modification_count = 0, \
            .track_change = false \
        },
    #define ENTRY(c, n, d, f) \
        { \
            .vm_cvar = &c, \
            .cvar_name = n, \
            .default_string = d, \
            .cvar_flags = f, \
            .modification_count = 0, \
            .track_change = false \
        },
    #define TRACK(c, n, d, f) \
        { \
            .vm_cvar = &c, \
            .cvar_name = n, \
            .default_string = d, \
            .cvar_flags = f, \
            .modification_count = 0, \
            .track_change = true \
        },

    #define A	CVAR_ARCHIVE
    #define C	CVAR_CHEAT
    #define L	CVAR_LATCH
    #define R	CVAR_ROM
    #define S	CVAR_SERVER_INFO
    #define T	CVAR_TEMP
    #define U	CVAR_USER_INFO
    #define W	CVAR_WOLF_INFO
    #define Y	CVAR_SYSTEM_INFO
    #define AL	(A | L)
    #define CS	(C | S)
    #define LS	(L | S)
    #define RS	(R | S)
    #define ALS	(A | L | S)
    #define ALSU (A | L | S | U)

    //
    // cheats
    //
    ENTRY(g_cheats, "sv_cheats", "", 0)

    //
    // blank vars
    //
    BLANK("gamename", BASEGAME, RS)
    BLANK("gamedate",	__DATE__, R)
    BLANK("sv_mapname", "", RS)

    //
    // latched vars
    //
    ENTRY(g_gametype, "g_gametype", "dm", LS)
    ENTRY(g_max_clients, "sv_maxclients", "20", ALS)

    //
    //
    //
    ENTRY(g_synchronous_clients, "g_synchronousClients", "0", S)
    ENTRY(g_intermission_delay, "g_intermissionDelay",	"1000", 0)

    //
    // logs
    //
    ENTRY(g_log, "g_log", "games_mp.log", A)
    ENTRY(g_log_sync, "g_logSync", "0", A)

    //
    // general server
    //
    ENTRY(g_password, "g_password", "", 0)
    ENTRY(g_ban_ips, "g_banIPs", "", A)
    ENTRY(g_dedicated, "dedicated", "0", 0)

    // 
    // physics
    //
    TRACK(g_speed, "g_speed", "190", 0)
    TRACK(g_gravity, "g_gravity", "800", 0)
    TRACK(g_knockback,	"g_knockback", "1000", 0)

    //
    // weapons
    //
    TRACK(g_weapon_respawn, "g_weaponrespawn", "5", 0)
    TRACK(g_weapon_ammo_pools, "g_weaponAmmoPools", "0", 0)

    #ifdef CODVERSION1_5
    ENTRY(g_max_dropped_weapons, "g_maxDroppedWeapons", "1", 0)
    #endif

    //
    // inactivity
    //
    TRACK(g_inactivity, "g_inactivity", "0", 0)

    //
    // debug
    //
    ENTRY(g_debug_move, "g_debugMove", "0", 0)
    ENTRY(g_debug_prone_check, "g_debugProneCheck", "0", 0)
    ENTRY(g_debug_prone_check_depth_check,	"g_debugProneCheckDepthCheck", "1", 0)
    ENTRY(g_debug_damage, "g_debugDamage", "0", C)
    ENTRY(g_debug_alloc, "g_debugAlloc", "0", 0)
    ENTRY(g_debug_bullets,	 "g_debugBullets", "0", C)

    //
    // general game stuff
    //
    ENTRY(g_motd, "g_motd", "", A)
    ENTRY(g_allow_vote, "g_allowVote",	"1", 0)

    #ifdef CODVERSION1_5
    ENTRY(g_allow_vote_map_restart, "g_allowVoteMapRestart", "1", AL)
    ENTRY(g_allow_vote_map_rotate, "g_allowVoteMapRotate", "1", AL)
    ENTRY(g_allow_vote_type_map, "g_allowVoteTypeMap", "1", AL)
    ENTRY(g_allow_vote_map, "g_allowVoteMap", "1", AL)
    ENTRY(g_allow_vote_gametype, "g_allowVoteGametype", "1", AL)
    ENTRY(g_allow_vote_kick, "g_allowVoteKick", "0", AL)
    ENTRY(g_allow_vote_client_kick, "g_allowVoteClientKick", "0", AL)
    ENTRY(g_allow_vote_temp_ban_user, "g_allowVoteTempBanUser", "0", AL)
    ENTRY(g_allow_vote_temp_ban_client, "g_allowVoteTempBanClient", "0", AL)
    ENTRY(g_allow_vote_draw_friend, "g_allowVoteDrawFriend", "0", AL)
    ENTRY(g_allow_vote_killcam, "g_allowVoteKillcam", "0", AL)
    ENTRY(g_allow_vote_friendly_fire, "g_allowVoteFriendlyFire", "0", AL)
    #endif

    ENTRY(g_list_entity, "g_listEntity", "0", 0)

    #ifdef CODVERSION1_5
    ENTRY(ui_allow_vote, "ui_allowVote", "1", Y)
    ENTRY(ui_allow_vote_map_restart, "ui_allowVoteMapRestart", "1", Y)
    ENTRY(ui_allow_vote_map_rotate, "ui_allowVoteMapRotate", "1", Y)
    ENTRY(ui_allow_vote_type_map, "ui_allowVoteTypeMap", "1", Y)
    ENTRY(ui_allow_vote_map, "ui_allowVoteMap", "1", Y)
    ENTRY(ui_allow_vote_gametype, "ui_allowVoteGametype", "1", Y)
    ENTRY(ui_allow_vote_kick, "ui_allowVoteKick", "0", Y)
    ENTRY(ui_allow_vote_client_kick, "ui_allowVoteClientKick", "0", Y)
    ENTRY(ui_allow_vote_temp_ban_user, "ui_allowVoteTempBanUser", "0", Y)
    ENTRY(ui_allow_vote_temp_ban_client, "ui_allowVoteTempBanClient", "0", Y)
    ENTRY(ui_allow_vote_draw_friend, "ui_allowVoteDrawFriend", "0", Y)
    ENTRY(ui_allow_vote_killcam, "ui_allowVoteKillcam", "0", Y)
    ENTRY(ui_allow_vote_friendly_fire, "ui_allowVoteFriendlyFire", "0", Y)
    ENTRY(scr_draw_friend, "scr_drawfriend", "0", AR)
    ENTRY(scr_friendly_fire, "scr_friendlyfire", "0", AR)
    ENTRY(scr_killcam, "scr_killcam", "0", AR)
    ENTRY(ui_draw_friend, "ui_drawfriend", "0", Y)
    ENTRY(ui_friendly_fire, "ui_friendlyfire", "0", Y)
    ENTRY(ui_killcam, "ui_killcam", "0", Y)
    #endif

    TRACK(g_complaint_limit, "g_complaintlimit", "3", A)
    ENTRY(g_voice_chats_allowed, "g_voiceChatsAllowed", "4", A)

    #ifdef CODVERSION1_5
    ENTRY(g_dead_chat, "g_deadChat", "0", A)
    #endif

    //
    // developer mode
    //
    ENTRY(g_developer, "developer", "0", T)

    //
    // score banners
    //
    ENTRY(g_scores_banner_allies, "g_scoresBanner_Allies", "gfx/hud/hud@mpflag_american.tga", W)
    ENTRY(g_scores_banner_axis, "g_scoresBanner_Axis", "gfx/hud/hud@mpflag_german.tga", W)
    ENTRY(g_scores_banner_none, "g_scoresBanner_None", "gfx/hud/hud@mpflag_none.tga", W)
    ENTRY(g_scores_banner_spectators, "g_scoresBanner_Spectators", "gfx/hud/hud@mpflag_spectator.tga", W)

    //
    // team names and colors
    //
    ENTRY(g_team_name_allies, "g_teamName_Allies", "GAME_ALLIES", W)
    ENTRY(g_team_name_axis, "g_teamName_Axis", "GAME_AXIS", W)
    ENTRY(g_team_color_allies, "g_teamColor_Allies", "0.5 0.5 1", W)
    ENTRY(g_team_color_axis, "g_teamColor_Axis", "1 0.5 0.5", W)

    // 
    // pmove
    //
    ENTRY(g_smoothclients, "g_smoothClients", "1", 0)
    ENTRY(pmove_fixed, "pmove_fixed", "0", Y)
    ENTRY(pmove_msec, "pmove_msec", "8", Y)

    //
    // main menu
    //
    ENTRY(g_script_main_menu, "g_scriptMainMenu", "", 0)

    //
    // bgame stuff
    //
    ENTRY(bg_view_height_standing, "bg_viewheight_standing", "60", W)
    ENTRY(bg_view_height_crouched, "bg_viewheight_crouched", "40", W)
    ENTRY(bg_view_height_prone, "bg_viewheight_prone", "11", W)

    // these are only present in 1.1
    #ifndef CODVERSION1_5
    ENTRY(bg_duck2prone_time, "bg_duck2prone_time", "400", W)
    ENTRY(bg_prone2duck_time, "bg_prone2duck_time", "400", W)
    #endif

    ENTRY(bg_ladder_yaw_cap, "bg_ladder_yawcap", "100", W)
    ENTRY(bg_prone_yaw_cap, "bg_prone_yawcap", "85", W)

    // also only present in 1.1
    #ifndef CODVERSION1_5
    ENTRY(bg_prone_soft_yaw_edge, "bg_prone_softyawedge", "1", W)
    #endif

    ENTRY(bg_foliagesnd_min_speed, "bg_foliagesnd_minspeed", "40", W)
    ENTRY(bg_foliagesnd_max_speed, "bg_foliagesnd_maxspeed", "180", W)
    ENTRY(bg_foliagesnd_slow_interval, "bg_foliagesnd_slowinterval", "1500", W)
    ENTRY(bg_foliagesnd_fast_interval, "bg_foliagesnd_fastinterval", "500", W)
    ENTRY(bg_foliagesnd_reset_interval, "bg_foliagesnd_resetinterval", "500", W)
    ENTRY(bg_fall_damage_min_height, "bg_fallDamageMinHeight", "256", CS)
    ENTRY(bg_fall_damage_max_height, "bg_fallDamageMaxHeight", "480", CS)
    ENTRY(bg_debug_weapon_anim, "bg_debugWeaponAnim", "0", C)
    ENTRY(bg_debug_weapon_state, "bg_debugWeaponState", "0", C)

    #ifdef CODVERSION1_5
    ENTRY(cg_bob_amplitude_standing, "cg_bobAmplitudeStanding", "0.007", W)
    ENTRY(cg_bob_amplitude_ducked, "cg_bobAmplitudeDucked", "0.0075", W)
    ENTRY(cg_bob_amplitude_prone, "cg_bobAmplitudeProne", "0.03", W)
    ENTRY(cg_bob_max, "cg_bobMax", "8", W)
    #endif

    ENTRY(g_bounds_width, "g_bounds_width", "30", C)
    ENTRY(g_bounds_height_standing, "g_bounds_height_standing", "70", C)

    //
    // misc
    //
    ENTRY(g_no_script_spam, "g_no_script_spam", "0", 0)
    ENTRY(g_debug_show_hit, "g_debugShowHit", "0", C)
    ENTRY(g_debug_loc_damage,"g_debugLocDamage", "0", C)
    ENTRY(bg_debug_anim, "bg_debuganim", "0", C)
    ENTRY(bg_swing_speed, "bg_swingSpeed", "0.2", C)
    ENTRY(g_use_gear, "g_useGear", "1", AL)
    ENTRY(g_language_warnings, "cl_languagewarnings", "0", 0)
    ENTRY(g_language_warnings_as_errors, "g_languagewarningsaserrors", "0", 0)
    ENTRY(g_dump_anims,"g_dumpAnims", "", C)

    #ifdef CODVERSION1_5
    ENTRY(g_auto_screenshot, "g_autoscreenshot", "0", A)
    ENTRY(&g_auto_demo, "g_autodemo", "0", A)

    //
    // timeouts
    //
    ENTRY(g_timeouts_allowed, "g_timeoutsallowed", "0", ALSU)
    ENTRY(g_timeout_bank, "g_timeoutBank", "180000", AL)
    ENTRY(g_timeout_length, "g_timeoutlength", "90000", AL)
    ENTRY(g_timeout_recovery, "g_timeoutRecovery", "10000", AL)
    ENTRY(ui_timeouts_allowed, "ui_timeoutsAllowed", "180000", W)
    ENTRY(ui_timeout_bank, "ui_timeoutBank", "180000", W)
    ENTRY(ui_timeout_length, "ui_timeoutLength", "90000", W)
    ENTRY(ui_timeout_recovery, "ui_timeoutRecovery", "10000", W)
    #endif

    #undef BLANK
    #undef ENTRY
    #undef TRACK
    #undef A
    #undef C
    #undef L
    #undef R
    #undef S
    #undef T
    #undef U
    #undef W
    #undef Y
    #undef AL
    #undef CS
    #undef LS
    #undef RS
    #undef ALS
    #undef ALSU
};

static int cvar_table_size = ARRAY_SIZE(cvar_table);
//
//
//
//
//

/**
 * @brief Register cvars with the server code
*/
void register_cvars(void)
{
    int i;
    struct cvar_table *cv;

    for (i = 0, cv = cvar_table; i < cvar_table_size; i++, cv++) {
        trap_cvar_register(cv->vm_cvar,
                           cv->cvar_name,
                           cv->default_string,
                           cv->cvar_flags);

        if (cv->vm_cvar)
            cv->modification_count = cv->vm_cvar->modification_count;
    }
    
    if (!is_valid_gametype(g_gametype.string)) {
        g_printf("g_gametype %s is not a valid gametype, defaulting to dm\n",
                 g_gametype.string);
        trap_cvar_set("g_gametype", "dm");
        trap_cvar_update(&g_gametype);
    }
}

/**
 * @brief Update any local copies of cvars with the server versions
*/
void update_cvars(void)
{
    int i;
    struct cvar_table *cv;

    for (i = 0, cv = cvar_table; i < cvar_table_size; i++, cv++) {
        if (cv->vm_cvar == NULL)
            continue;

        trap_cvar_update(cv->vm_cvar);

        if (cv->modification_count != cv->vm_cvar->modification_count) {
            cv->modification_count = cv->vm_cvar->modification_count;

            if (cv->track_change) {
                trap_send_server_command(-1, 0, va("e \"GAME_SERVER\x15: %s " \
                                                   "\x14GAME_CHANGEDTO\x15 %s\"",
                                                   cv->cvar_name,
                                                   cv->vm_cvar->string));
            }
        }
    }
}
