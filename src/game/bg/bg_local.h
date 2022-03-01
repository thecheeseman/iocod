#ifndef __BG_LOCAL_H__
#define __BG_LOCAL_H__

#include "../game_local.h"

#define BG_WEAPONS_FOLDER "weapons/mp"

enum team {
	TEAM_NONE,
	TEAM_AXIS,
	TEAM_ALLIES,
	TEAM_SPECTATOR,
	MAX_TEAMS
};

enum session_state {
	STATE_PLAYING,
	STATE_DEAD,
	STATE_SPECTATOR,
	STATE_INTERMISSION
};

enum means_of_death {
	MOD_UNKNOWN,
	MOD_PISTOL_BULLET,
	MOD_RIFLE_BULLET,
	MOD_GRENADE,
	MOD_GRENADE_SPLASH,
	MOD_PROJECTILE,
	MOD_PROJECTILE_SPLASH,
	MOD_MELEE,
	MOD_HEAD_SHOT,
	MOD_MORTAR,
	MOD_MORTAR_SPLASH,
	MOD_KICKED,
	MOD_GRABBER,
	MOD_DYNAMITE,
	MOD_AIRSTRIKE,
	MOD_WATER,
	MOD_SLIME,
	MOD_LAVA,
	MOD_CRUSH,
	MOD_TELEFRAG,
	MOD_FALLING,
	MOD_SUICIDE,
	MOD_TRIGGER_HURT,
	MOD_EXPLOSIVE
};

enum item_type {
	IT_BAD,
	IT_WEAPON,
	IT_AMMO,
	IT_HEALTH
};

struct gitem {
	char *classname;
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

#define WEAPONFILE_HEADER "WEAPONFILE"
#define MAX_WEAPONFILE_SIZE 8192

enum weapon_type {
	WEAPON_TYPE_BULLET,
	WEAPON_TYPE_GRENADE,
	WEAPON_TYPE_PROJECTILE
};

enum overlay_reticle {
	OVERLAY_RETICLE_NONE,
	OVERLAY_RETICLE_CROSSHAIR,
	OVERLAY_RETICLE_FG42,
	OVERLAY_RETICLE_SPRINGFIELD,
	OVERLAY_RETICLE_GEHWEHR43
};

enum weapon_class {
	WEAPON_CLASS_RIFLE,
	WEAPON_CLASS_MG,
	WEAPON_CLASS_SMG,
	WEAPON_CLASS_PISTOL,
	WEAPON_CLASS_GRENADE,
	WEAPON_CLASS_ROCKET_LAUNCHER,
	WEAPON_CLASS_TURRET,
	WEAPON_CLASS_NON_PLAYER
};

enum weapon_slot {
	WEAPON_SLOT_NONE,
	WEAPON_SLOT_PRIMARYA,
	WEAPON_SLOT_PRIMARYB,
	WEAPON_SLOT_PISTOL,
	WEAPON_SLOT_GRENADE,
	WEAPON_SLOT_SMOKEGRENADE,
	WEAPON_SLOT_MAX
};

enum weapon_stance {
	WEAPON_STANCE_STAND,
	WEAPON_STANCE_DUCK,
	WEAPON_STANCE_PRONE
};

enum projectile_type {
	PROJECTILE_TYPE_GRENADE,
	PROJECTILE_TYPE_ROCKET,
	PROJECTILE_TYPE_MOLOTOV,
	PROJECTILE_TYPE_NONE
};

enum weapon_file_types {
	WFT_STRING,

	// unused
	WFT_STRING2,
	WFT_STRING3,
	WFT_STRING4,
	// unused

	WFT_INT,
	WFT_BOOL,
	WFT_FLOAT,
	WFT_TIME,

	WFT_WEAPON_TYPE,
	WFT_WEAPON_CLASS,
	WFT_OVERLAY_RETICLE,
	WFT_WEAPON_SLOT,
	WFT_WEAPON_STANCE,
	WFT_PROJECTILE_TYPE,

	WFT_TYPE_MAX
};

struct weapon {
	int number;
	char *name;
	char *display_name;
	char *ai_overlay_description;

	char *gun_model;
	char *hand_model;

	int field_0x18;

	char *idle_anim;
	char *empty_idle_anim;
	char *fire_anim;
	char *hold_fire_anim;
	char *last_shot_anim;
	char *rechamber_anim;
	char *melee_anim;
	char *reload_anim;
	char *reload_empty_anim;
	char *reload_start_anim;
	char *reload_end_anim;
	char *raise_anim;
	char *drop_anim;
	char *alt_raise_anim;
	char *alt_drop_anim;
	char *ads_fire_anim;
	char *ads_last_shot_anim;
	char *ads_rechamber_anim;
	char *ads_up_anim;
	char *ads_down_anim;
	char *mode_name;

	enum weapon_type weapon_type;
	enum weapon_class weapon_class;
	enum weapon_slot weapon_slot;
	bool slot_stackable;

	enum weapon_stance stance;

	char *view_flash_effect;
	char *world_flash_effect;

	char *pickup_sound;
	char *ammo_pickup_sound;
	char *projectile_sound;
	char *pullback_sound;
	char *fire_sound;
	char *loop_fire_sound;
	char *stop_fire_sound;
	char *fire_echo_sound;
	char *last_shot_sound;
	char *rechamber_sound;
	char *reload_sound;
	char *reload_empty_sound;
	char *reload_start_sound;
	char *reload_end_sound;
	char *raise_sound;
	char *alt_switch_sound;
	char *put_away_sound;
	char *note_track_sound_a;
	char *note_track_sound_b;
	char *note_track_sound_c;
	char *note_track_sound_d;

	char *shell_eject_effect;
	char *last_shot_eject_effect;

	char *reticle_center;
	char *reticle_side;
	int reticle_center_size;
	int reticle_side_size;
	int reticle_min_ofs;

	float stand_move_f;
	float stand_move_r;
	float stand_move_u;
	float stand_rot_p;
	float stand_rot_y;
	float stand_rot_r;
	float ducked_of_sf;
	float ducked_of_sr;
	float ducked_of_su;
	float ducked_move_f;
	float ducked_move_r;
	float ducked_move_u;
	float ducked_rot_p;
	float ducked_rot_y;
	float ducked_rot_r;
	float prone_of_sf;
	float prone_of_sr;
	float prone_of_su;
	float prone_move_f;
	float prone_move_r;
	float prone_move_u;
	float prone_rot_p;
	float prone_rot_y;
	float prone_rot_r;
	float pos_move_rate;
	float pos_prone_move_rate;
	float stand_move_min_speed;
	float ducked_move_min_speed;
	float prone_move_min_speed;
	float pos_rot_rate;
	float pos_prone_rot_rate;
	float stand_rot_min_speed;
	float ducked_rot_min_speed;
	float prone_rot_min_speed;

	char *radiant_name;
	char *world_model;
	char *hud_icon;
	char *mode_icon;
	char *ammo_icon;

	int start_ammo;
	char *ammo_name;
	int ammo_name_index;

	char *clip_name;
	int clip_name_index;
	int max_ammo;
	int clip_size;

	char *shared_ammo_cap_name;
	int shared_ammo_cap_index;
	int shared_ammo_cap;

	int damage;
	int melee_damage;

	int field_0x1c8;

	int fire_delay;
	int melee_delay;
	int fire_time;
	int rechamber_time;
	int rechamber_bolt_time;
	int hold_fire_time;
	int melee_time;
	int reload_time;
	int reload_empty_time;
	int reload_add_time;
	int reload_start_time;
	int reload_start_add_time;
	int reload_end_time;
	int drop_time;
	int raise_time;
	int alt_drop_time;
	int alt_raise_time;
	int fuse_time;

	float move_speed_scale;

	float ads_zoom_fov;
	float ads_zoom_in_frac;
	float ads_zoom_out_frac;
	char *ads_overlay_shader;
	enum overlay_reticle ads_overlay_reticle;
	float ads_overlay_width;
	float ads_overlay_height;
	float ads_bob_factor;
	float ads_view_bob_mult;

	float hip_spread_stand_min;
	float hip_spread_ducked_min;
	float hip_spread_prone_min;
	float hip_spread_max;
	float hip_spread_decay_rate;
	float hip_spread_fire_add;
	float hip_spread_turn_add;
	float hip_spread_move_add;
	float hip_spread_ducked_decay;
	float hip_spread_prone_decay;
	float hip_reticle_side_pos;

	int ads_trans_in_time;
	int ads_trans_out_time;

	float ads_idle_amount;
	float hip_idle_amount;
	float idle_crouch_factor;
	float idle_prone_factor;

	float gun_max_pitch;
	float gun_max_yaw;
	float sway_max_angle;
	float sway_lerp_speed;
	float sway_pitch_scale;
	float sway_yaw_scale;
	float sway_horiz_scale;
	float sway_vert_scale;
	float sway_shellshock_scale;
	float ads_sway_max_angle;
	float ads_sway_lerp_speed;
	float ads_sway_pitch_scale;
	float ads_sway_yaw_scale;
	float ads_sway_horiz_scale;
	float ads_sway_vert_scale;

	bool two_handed;
	bool rifle_bullet;
	bool semi_auto;
	bool bolt_action;
	bool aim_down_sight;

	#ifdef CODVERSION1_5
	bool rechamber_while_aiming;
	float ads_view_error_min;
	float ads_view_error_max;
	#endif

	bool cook_off_hold;
	bool clip_only;
	bool wide_list_icon;
	bool ads_fire;

	char *kill_icon;

	bool wide_kill_icon;
	bool no_partial_reload;
	bool segmented_reload;

	int reload_ammo_add;
	int reload_start_add;

	char *alt_weapon;
	int alt_weapon_index;

	int drop_ammo_min;
	int drop_ammo_max;

	int explosion_radius;
	int explosion_inner_damage;
	int explosion_outer_damage;

	int projectile_speed;
	int projectile_speed_up;
	char *projectile_model;
	enum projectile_type projectile_explosion_type;
	char *projectile_explosion_effect;
	char *projectile_explosion_sound;
	bool projectile_impact_explode;
	char *projectile_trail_effect;
	int projectile_dlight;
	float projectile_red;
	float projectile_green;
	float projectile_blue;

	float ads_aim_pitch;
	float ads_crosshair_in_frac;
	float ads_crosshair_out_frac;
	float ads_gun_kick_pitch_min;
	float ads_gun_kick_pitch_max;
	float ads_gun_kick_yaw_min;
	float ads_gun_kick_yaw_max;
	float ads_gun_kick_accel;
	float ads_gun_kick_speed_max;
	float ads_gun_kick_speed_decay;
	float ads_gun_kick_static_decay;
	float ads_view_kick_pitch_min;
	float ads_view_kick_pitch_max;
	float ads_view_kick_yaw_min;
	float ads_view_kick_yaw_max;
	float ads_view_kick_center_speed;

	int field_0x384;
	int field_0x388;

	float ads_spread;

	float hip_gun_kick_pitch_min;
	float hip_gun_kick_pitch_max;
	float hip_gun_kick_yaw_min;
	float hip_gun_kick_yaw_max;
	float hip_gun_kick_accel;
	float hip_gun_kick_speed_max;
	float hip_gun_kick_speed_decay;
	float hip_gun_kick_speed_static;
	float hip_view_kick_pitch_min;
	float hip_view_kick_pitch_max;
	float hip_view_kick_yaw_min;
	float hip_view_kick_yaw_max;
	float hip_view_kick_center_speed;

	int field_0x3c4;
	int field_0x3c8;

	float ai_effective_range;
	float ai_miss_range;

	int ads_reload_trans_time;
	int ads_trans_blend_time;

	float left_arc;
	float right_arc;
	float top_arc;
	float bottom_arc;
	float accuracy;
	float vert_turn_speed;
	float hor_turn_speed;
	float convergence_time;
	float max_range;
	float anim_hor_rotate_inc;
	float player_position_dist;

	char *use_hint_string;
	int hint_string_index;

	char *script;

	float ads_trans_in_time_f;
	float ads_trans_out_time_f;

	#ifdef CODVERSION1_5
	float horiz_view_jitter;
	float vert_view_jitter;
	#endif
};

extern struct weapon **bg_weapons;
extern int bg_num_weapons;

void bg_set_up_weapon_info(void);

#endif // __BG_LOCAL_H__
