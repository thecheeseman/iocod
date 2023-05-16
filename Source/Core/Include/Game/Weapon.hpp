// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_WEAPON_HPP
#define GAME_WEAPON_HPP

#include <string_view>

namespace iocod {

enum class WeaponType {
    Bullet,
    Grenade,
    Projectile
};

enum class WeaponClass {
    Rifle,
    MG,
    SMG,
    Pistol,
    Grenade,
    RocketLauncher,
    Turret,
    NonPlayer
};

enum class WeaponSlot {
    None,
    PrimaryA,
    PrimaryB,
    Pistol,
    Grenade,
    SmokeGrenade
};

enum class WeaponStance {
    Stand,
    Duck,
    Prone
};

enum class ProjectileType {
    None,
    Grenade,
    Rocket,
    Molotov
};

enum class OverlayReticle {
    None,
    Crosshair,
    FG42,
    Springfield,
    Gewehr43
};

struct Weapon {
    std::string_view display_name;
    std::string_view ai_overlay_description;

    std::string_view mode_name;
    std::string_view gun_model;
    std::string_view hand_model;

	std::string_view idle_anim;
    std::string_view empty_idle_anim;
    std::string_view fire_anim;
    std::string_view hold_fire_anim;
    std::string_view last_shot_anim;
    std::string_view rechamber_anim;
    std::string_view melee_anim;
    std::string_view reload_anim;
    std::string_view reload_empty_anim;
    std::string_view reload_start_anim;
    std::string_view reload_end_anim;
    std::string_view raise_anim;
    std::string_view drop_anim;
    std::string_view alt_raise_anim;
    std::string_view alt_drop_anim;
    std::string_view ads_fire_anim;
    std::string_view ads_last_shot_anim;
    std::string_view ads_rechamber_anim;
    std::string_view ads_up_anim;
    std::string_view ads_down_anim;

    std::string_view script;

    WeaponType weapon_type;
    WeaponClass weapon_class;
    WeaponSlot weapon_slot;

    bool slot_stackable;

    std::string_view view_flash_effect;
    std::string_view world_flash_effect;

    std::string_view pickup_sound;
    std::string_view ammo_pickup_sound;
    std::string_view projectile_sound;
    std::string_view pullback_sound;
    std::string_view fire_sound;
    std::string_view loop_fire_sound;
    std::string_view stop_fire_sound;
    std::string_view fire_echo_sound;
    std::string_view last_shot_sound;
    std::string_view rechamber_sound;
    std::string_view reload_sound;
    std::string_view reload_empty_sound;
    std::string_view reload_start_sound;
    std::string_view reload_end_sound;
    std::string_view raise_sound;
    std::string_view alt_switch_sound;
    std::string_view put_away_sound;
    std::string_view note_track_sound_a;
    std::string_view note_track_sound_b;
    std::string_view note_track_sound_c;
    std::string_view note_track_sound_d;

    std::string_view shell_eject_effect;
    std::string_view last_shot_eject_effect;

    std::string_view reticle_center;
    std::string_view reticle_side;
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

    std::string_view radiant_name;
    std::string_view world_model;
    std::string_view hud_icon;
    std::string_view mode_icon;
    std::string_view ammo_icon;

    int start_ammo;
    std::string_view ammo_name;
    std::string_view clip_name;
    int max_ammo;
    int clip_size;
    std::string_view shared_ammo_cap_name;
    int shared_ammo_cap;
    int damage;
    int melee_damage;

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

    bool rifle_bullet;
    bool two_handed;
    bool semi_auto;
    bool bolt_action;
    bool aim_down_sight;

    // 1.5
    bool rechamber_while_aiming;
    float ads_view_error_min;
    float ads_view_error_max;
    // 1.5

    bool clip_only;
    bool cook_off_hold;
    bool wide_list_icon;
    bool ads_fire;

    std::string_view kill_icon;
    bool wide_kill_icon;
    bool no_partial_reload;
    bool segmented_reload;
    int reload_ammo_add;
    int reload_start_add;
    std::string_view alt_weapon;

	int drop_ammo_min;
    int drop_ammo_max;

    int explosion_radius;
    int explosion_inner_damage;
    int explosion_outer_damage;

    int projectile_speed;
    int projectile_speed_up;
    std::string_view projectile_model;
    ProjectileType projectile_explosion_type;
    std::string_view projectile_explosion_effect;
    std::string_view projectile_explosion_sound;
    bool projectile_impact_explode;
    std::string_view projectile_trail_effect;
    int projectile_dlight;
    float projectile_red;
    float projectile_green;
    float projectile_blue;

    int ads_trans_in_time;
    int ads_trans_out_time;
    float ads_idle_amount;

	float ads_zoom_fov;
    float ads_zoom_in_frac;
    float ads_zoom_out_frac;
    std::string_view ads_overlay_shader;
    OverlayReticle ads_overlay_reticle;
    float ads_overlay_width;
    float ads_overlay_height;
    float ads_bob_factor;
    float ads_view_bob_mult;

	float ads_aim_pitch;
    float ads_crosshair_in_frac;
    float ads_crosshair_out_frac;

    int ads_reload_trans_time;
    int ads_trans_blend_time;

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

    float ads_spread;

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

    float hip_idle_amount;

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

    WeaponStance stance;

    std::string_view use_hint_string;

    // 1.5
    float horiz_view_jitter;
    float vert_view_jitter;
    // 1.5

    float ai_effective_range;
    float ai_miss_range;
};

} // namespace iocod

#endif // GAME_WEAPON_HPP
