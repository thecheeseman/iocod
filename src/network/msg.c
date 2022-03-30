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
 * @file msg.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"

void INCOMPLETE msg_init(struct msg *buf, byte *data, int length)
{
    (void) buf; (void) data; (void) length; //-Wunused-parameter
}

struct netfield {
    char *name;
    intptr_t offset;
    int bit; // 0 = float
};

// black magic
#define NETF(x) #x,(intptr_t)&((struct entity_state *)0)->x

struct netfield entity_state_fields[] = {
    { NETF(pos.time), 32 },
    { NETF(pos.base[0]), 0 },
    { NETF(pos.base[1]), 0 },
    { NETF(pos.delta[1]), 0 },
    { NETF(pos.delta[0]), 0 },
    { NETF(angles2[1]), 0 },
    { NETF(apos.base[1]), 0 },
    { NETF(apos.base[0]), 0 },
    { NETF(pos.base[2]), 0 },
    { NETF(pos.delta[2]), 0 },
    { NETF(event_sequence), 8 },
    { NETF(entity_type), 8 },
    { NETF(entity_flags), 24 },
    { NETF(ground_entity_num), 10 },
    { NETF(legs_anim), 10 },
    { NETF(client_num), 8 },
    { NETF(apos.base[2]), 0 },
    { NETF(events[0]), 8 },
    { NETF(index), 9 },
    { NETF(events[1]), 8 },
    { NETF(events[2]), 8 },
    { NETF(events[3]), 8 },
    { NETF(event_param), 8 },
    { NETF(torso_anim), 10 },
    { NETF(surface_type), 8 },
    { NETF(_union.scale), 8 },
    { NETF(other_entity_num), 10 },
    { NETF(waist_pitch), 0 },
    { NETF(pos.type), 8 },
    { NETF(angles2[0]), 0 },
    { NETF(torso_height), 0 },
    { NETF(torso_pitch), 0 },
    { NETF(apos.type), 8 },
    { NETF(solid), 24 },
    { NETF(weapon), 6 },
    { NETF(apos.time), 32 },
    { NETF(apos.delta[0]), 0 },
    { NETF(event_params[0]), 8 },
    { NETF(pos.duration), 32 },
    { NETF(anim_move_type), 4 },
    { NETF(event_params[1]), 8 },
    { NETF(apos.delta[2]), 0 },
    { NETF(event_params[2]), 8 },
    { NETF(event_params[3]), 8 },
    { NETF(lean), 0 },
    { NETF(apos.delta[1]), 0 },
    { NETF(loop_sound), 8 },
    { NETF(attacker_entity_num), 10 },
    { NETF(headicon), 4 },
    { NETF(headicon_team), 2 },
    { NETF(apos.duration), 32 },
    { NETF(time), 32 },
    { NETF(time2), 32 },
    { NETF(origin2[0]), 0 },
    { NETF(origin2[1]), 0 },
    { NETF(origin2[2]), 0 },
    { NETF(angles2[2]), 0 },
    { NETF(constant_light), 32 },
    { NETF(damage_flags), 32 }
};

// black magic
#define PSF(x) #x,(intptr_t)&((struct player_state *)0)->x

struct netfield player_state_fields[] = {
    { PSF(command_time), 32 },
    { PSF(origin[1]), 0 },
    { PSF(origin[0]), 0 },
    { PSF(origin[2]), 0 },
    { PSF(view_angles[1]), 0 },
    { PSF(view_angles[0]), 0 },
    { PSF(compass_friend_info), 32 },
    { PSF(entity_flags), 24 },
    { PSF(bob_cycle), 8 },
    { PSF(view_height_current), 0 },
    { PSF(event_sequence), 8 },
    { PSF(legs_anim), 10 },
    { PSF(pm_flags), 21 },
    { PSF(delta_angles[1]), 16 },
    { PSF(velocity[1]), 0 },
    { PSF(velocity[0]), 0 },
    { PSF(speed), 16 },
    { PSF(mins[0]), 0 },
    { PSF(mins[1]), 0 },
    { PSF(maxs[0]), 0 },
    { PSF(maxs[1]), 0 },
    { PSF(maxs[2]), 0 },
    { PSF(prone_view_height), -8 },
    { PSF(crouch_view_height), -8 },
    { PSF(stand_view_height), -8 },
    { PSF(dead_view_height), -8 },
    { PSF(walk_speed_scale), 0 },
    { PSF(run_speed_scale), 0 },
    { PSF(prone_speed_scale), 0 },
    { PSF(crouch_speed_scale), 0 },
    { PSF(strafe_speed_scale), 0 },
    { PSF(back_speed_scale), 0 },
    { PSF(lean_speed_scale), 0 },
    { PSF(friction), 0 },
    { PSF(ground_entity_num), 10 },
    { PSF(view_height_target), -8 },
    { PSF(events[0]), 8 },
    { PSF(weapons[0]), 32 },
    { PSF(weapon_slots[0]), 32 },
    { PSF(gravity), 16 },
    { PSF(server_cursor_hint_string), -8 },
    { PSF(events[1]), 8 },
    { PSF(events[2]), 8 },
    { PSF(events[3]), 8 },
    { PSF(delta_angles[0]), 16 },
    { PSF(weapon), 6 },
    { PSF(movement_dir), 8 },
    { PSF(view_height_lerp_target), -8 },
    { PSF(weapon_slots[4]), 32 },
    { PSF(prone_direction), 0 },
    { PSF(aim_spread_scale), 0 },
    { PSF(weap_anim), 10 },
    { PSF(jump_time), 32 },
    { PSF(velocity[2]), 0 },
    { PSF(weapon_time), -16 },
    { PSF(prone_torso_pitch), 0 },
    { PSF(prone_direction_pitch), 0 },
    { PSF(torso_pitch), 0 },
    { PSF(waist_pitch), 0 },
    { PSF(torso_height), 0 },
    { PSF(weapon_state), 4 },
    { PSF(torso_timer), 16 },
    { PSF(torso_anim), 10 },
    { PSF(event_params[0]), 8 },
    { PSF(ladder_angles[0]), 0 },
    { PSF(event_params[3]), 8 },
    { PSF(view_height_lerp_down), 1 },
    { PSF(weapon_delay), -16 },
    { PSF(event_params[1]), 8 },
    { PSF(view_height_lerp_time), 32 },
    { PSF(event_params[2]), 8 },
    { PSF(ladder_angles[1]), 0 },
    { PSF(ads_fraction), 0 },
    { PSF(pm_type), 8 },
    { PSF(legs_timer), 16 },
    { PSF(jump_origin_z), 0 },
    { PSF(lean), 0 },
    { PSF(damage_event), 8 },
    { PSF(damage_yaw), 8 },
    { PSF(damage_pitch), 8 },
    { PSF(damage_count), 7 },
    { PSF(weapon_rechamber[0]), 32 },
    { PSF(grenade_time_left), -16 },
    { PSF(pm_time), -16 },
    { PSF(foliage_sound_time), 32 },
    { PSF(delta_time), 32 },
    { PSF(server_cursor_hint), 8 },
    { PSF(server_cursor_hint_value), 8 },
    { PSF(shellshock_index), 4 },
    { PSF(shellshock_time), 32 },
    { PSF(shellshock_duration), 16 },
    { PSF(delta_angles[2]), 16 },
    { PSF(ladder_angles[2]), 0 },
    { PSF(client_num), 8 },
    { PSF(weapons[1]), 32 },
    { PSF(weapon_rechamber[1]), 32 },
    { PSF(view_angles[2]), 0 },
    { PSF(view_height_lerp_pos_adj), 0 },
    { PSF(mins[2]), 0 },
    { PSF(view_locked), 8 },
    { PSF(view_locked_ent_num), 10 },
    { PSF(gun_fx), 8 }
};

// TEMP
void INCOMPLETE huff_decompress(struct msg *msg, int a)
{
    UNUSED(msg);
    UNUSED(a);
}

void INCOMPLETE msg_begin_reading_oob(struct msg *msg)
{
    UNUSED(msg);
}

char * INCOMPLETE msg_read_string_line(struct msg *msg)
{
    UNUSED(msg);

    return "";
}

/**
 * @brief 
 * @param msg 
 * @return 
*/
int msg_read_long(struct msg *msg)
{
    int c, v;

    v = msg->readcount + 4;
    if (v > msg->cursize) {
        c = -1;
    } else {
        c = msg->data[msg->readcount];
        msg->readcount = v;
    }

    return c;
}
