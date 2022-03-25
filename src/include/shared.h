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
 * @file shared.h
 * @date 2022-02-04
*/

#ifndef __SHARED_H__
#define __SHARED_H__

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

// from kernel.h
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))
//

#define UNUSED(x) (void)(x)
#define INCOMPLETE

#define Q3_VERSION "iocod"
#define BASEGAME "main"
#define BASEDEMO "demomain" // not really necessary

#include "platform.h"

// typedefs
typedef int filehandle;
#include "types/byte.h"
#include "types/bool.h"
#include "types/vector.h"

// from ioquake
#define PAD(base, alignment)	(((base)+(alignment)-1) & ~((alignment)-1))
#define PADLEN(base, alignment)	(PAD((base), (alignment)) - (base))
#define PADP(base, alignment)	((void *) PAD((int) (base), (alignment)))

#define MAX_FILE_HANDLES 64

// TODO: increase this ?
#define MAX_QINT            0x7fffffff
#define MIN_QINT            (-MAX_QINT - 1)

// TTimo gcc: was missing, added from Q3 source
#ifndef max
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif

// angle indexes
#define PITCH               0       // up / down
#define YAW                 1       // left / right
#define ROLL                2       // fall over

enum print_level {
    PRINT_ALL,
    PRINT_DEVELOPER,
    PRINT_WARNING,
    PRINT_ERROR,
    PRINT_LOGONLY
};

enum fs_mode {
    FS_READ,
    FS_WRITE,
    FS_APPEND,
    FS_APPEND_SYNC
};

enum fs_origin {
    FS_SEEK_CUR,
    FS_SEEK_END,
    FS_SEEK_SET
};

// the game guarantees that no string from the network will ever
// exceed MAX_STRING_CHARS
#define MAX_STRING_CHARS    1024    // max length of a string passed to Cmd_TokenizeString
#define MAX_STRING_TOKENS   256     // max tokens resulting from Cmd_TokenizeString
#define MAX_TOKEN_CHARS     1024    // max length of an individual token

#define MAX_INFO_STRING     1024
#define MAX_INFO_KEY        1024
#define MAX_INFO_VALUE      1024

#define BIG_INFO_STRING     8192    // used for system info key only
#define BIG_INFO_KEY        8192
#define BIG_INFO_VALUE      8192

#define MAX_QPATH           64      // max length of a quake game pathname
#define MAX_OSPATH          256     // max length of a filesystem pathname

#define MAX_NAME_LENGTH     32      // max length of a client name

#define MAX_SAY_TEXT        150

#define MAX_PRINT_MSG		4096

struct usercmd {
    int server_time;

    byte buttons;
    byte wbuttons;
    byte weapon;
    byte flags;

    int angles[3];

    char forward_move, right_move, up_move;

    byte unknown;
};

void com_sprintf(char *dest, int size, const char *fmt, ...);
void com_default_extension(char *path, int maxsize, const char *extension);
int q_stricmpn(const char *s1, const char *s2, int n);
int q_stricmp(const char *s1, const char *s2);
int q_strncmp(const char *s1, const char *s2, int n);
void q_strncpyz(char *dest, const char *src, int destsize);
void q_strcat(char *dest, int size, const char *src);
char *q_strlwr(char *s1);
char *q_strupr(char *s1);

char *va(char *format, ...);

// TODO: maybe just make this something modern?
typedef struct
{
    byte b0;
    byte b1;
    byte b2;
    byte b3;
    byte b4;
    byte b5;
    byte b6;
    byte b7;
} qint64;

short   bigshort(short l);
short   littleshort(short l);
int     biglong(int l);
int     littlelong(int l);
qint64  biglong64(qint64 l);
qint64  littlelong64(qint64 l);
float   bigfloat(float l);
float   littlefloat(float l);

void    swap_init(void);

#define	SNAPFLAG_RATE_DELAYED	1
#define	SNAPFLAG_NOT_ACTIVE		2	// snapshot used during connection and for zombies
#define SNAPFLAG_SERVERCOUNT	4	// toggled every map_restart so transitions can be detected

//
// per-level limits
//
#define	MAX_CLIENTS			64		// absolute limit
#define MAX_LOCATIONS		64

#define	GENTITYNUM_BITS		10		// don't need to send any more
#define	MAX_GENTITIES		(1<<GENTITYNUM_BITS)

// entitynums are communicated with GENTITY_BITS, so any reserved
// values that are going to be communcated over the net need to
// also be in this range
#define	ENTITYNUM_NONE			(MAX_GENTITIES-1)
#define	ENTITYNUM_WORLD			(MAX_GENTITIES-2)
#define	ENTITYNUM_MAX_NORMAL	(MAX_GENTITIES-2)

#define MAX_CONFIG_STRINGS		2048

#define MAX_STATUS_ICONS		8
#define MAX_HEAD_ICONS			15
#define MAX_TAG_CONNECTS		32
#define MAX_CLIENT_VARS			64
#define MAX_MODELS				256
#define MAX_SOUNDS				256
#define MAX_FX					64
#define MAX_FX_ON_TAG			256
#define MAX_SHELLSHOCKS			16
#define MAX_MENUS				32
#define MAX_HINT_STRINGS		32
#define MAX_LOCALIZED_STRINGS	256
#define MAX_SHADERS				128
#define MAX_WEAPONS				64

#define CS_SERVER_INFO			0
#define CS_SYSTEM_INFO			1

#define HEARTBEAT_MSEC		 180000
#define HEARTBEAT_GAME		"COD-1"
#define HEARTBEAT_SHUTDOWN	"flatline"

enum trajectory_type {
    TR_STATIONARY,
    TR_INTERPOLATE,
    TR_LINEAR,
    TR_LINEAR_STOP,
    TR_SINE,
    TR_GRAVITY
};

typedef struct {
    enum trajectory_type type;
    int time;
    int duration;
    vec3_t base;
    vec3_t delta;
} trajectory_t;

struct entity_state {
    int number;                 // 0

    int entity_type;			// 4
    int entity_flags;           // 8

    trajectory_t pos;           // 12
    trajectory_t apos;          // 48

    int time;                   // 84
    int time2;                  // 88

    vec3_t origin2;             // 92
    vec3_t angles2;				// 104

    int other_entity_num;       // 116
    int attacker_entity_num;    // 120
    int ground_entity_num;      // 124

    int constant_light;         // 128

    int loop_sound;				// 132

    int surface_type;			// 136
    int index;					// 140

    int client_num;             // 144

    int headicon;               // 148
    int headicon_team;          // 152

    int solid;              	// 156

    int event_param;			// 160
    int event_sequence;         // 164
    int events[4];              // 168
    int event_params[4];        // 184

    int weapon;                 // 200

    int legs_anim;              // 204
    int torso_anim;             // 208

    float lean;					// 212

    // wtf is this
    union {
        int scale;
        int loop_fx_id;			// maybe this is here??
    } _union;

    int damage_flags;			// 220
    int anim_move_type;         // 224

    // 
    // entity_shared might be smaller than we think
    float torso_height;			// 228
    float torso_pitch;			// 232
    float waist_pitch;			// 236
};

struct entity_shared {
    // this is how q3 has it
    // struct entity_state s;

    bool linked;

    int sv_flags;

    int single_client;

    bool bmodel;

    vec3_t mins;
    vec3_t maxs;

    int contents;

    vec3_t abs_min;
    vec3_t abs_max;

    vec3_t current_origin;
    vec3_t current_angles;

    int owner_num;

    int event_time;

    int world_flags;
};

// actual size is
// 1.1 = 0x2100
// 1.5 = 0x2103
struct player_state {
    int command_time;			// 0

    int pm_type;				// 4
    int bob_cycle;				// 8
    int pm_flags;				// 12
    int pm_time;				// 16

    vec3_t origin;				// 20, 24, 28
    vec3_t velocity;			// 32, 36, 40

    int weapon_time;			// 44
    int weapon_delay;			// 48
    int grenade_time_left;		// 52

    int foliage_sound_time;		// 56
    
    int gravity;				// 60
    float lean;					// 64
    int speed;					// 68

    int delta_angles[3];		// 72, 76, 80
    int ground_entity_num;		// 84

    vec3_t ladder_angles;		// 88, 92, 96

    int jump_time;				// 100
    float jump_origin_z;		// 104

    int legs_timer;				// 108
    int legs_anim;				// 112
    int torso_timer;			// 116
    int torso_anim;				// 120
    int movement_dir;			// 124
    int entity_flags;			// 128

    int event_sequence;			// 132
    int events[4];				// 136, 140, 144, 148
    int event_params[4];		// 152, 156, 160, 164
    int old_event_sequence;		// 168

    int client_num;				// 172

    int weapon;					// 176
    int weapon_state;			// 180
    float ads_fraction;			// 184

    int viewmodel_index;		// 188

    vec3_t view_angles;			// 192, 196, 200

    int view_height_target;		// 204
    float view_height_current;	// 208
    int view_height_lerp_time;	// 212
    int view_height_lerp_target;// 216
    int view_height_lerp_down;	// 220
    float view_height_lerp_pos_adj;// 224

    int damage_event;			// 228
    int damage_yaw;				// 232
    int damage_pitch;			// 236
    int damage_count;			// 240

    int health;					// 244
    int death_yaw;				// 248
    int max_health;				// 252

    int field_256;				// 256
    int field_260;				// 260

    int times_spawned;			// 264

    int ammo[MAX_WEAPONS];		// 268
    int ammo_clip[MAX_WEAPONS];	// 524

    byte weapons[8];			// 780
    byte weapon_slots[8];		// 788
    byte weapon_rechamber[8];	// 796

    vec3_t mins;				// 804, 808, 812
    vec3_t maxs;				// 816, 820, 824

    int prone_view_height;		// 828
    int crouch_view_height;		// 832
    int stand_view_height;		// 836
    int dead_view_height;		// 840

    float walk_speed_scale;		// 844
    float run_speed_scale;		// 848
    float prone_speed_scale;	// 852
    float crouch_speed_scale;	// 856
    float strafe_speed_scale;	// 860
    float back_speed_scale;		// 864
    float lean_speed_scale;		// 868

    float prone_direction;		// 872
    float prone_direction_pitch;// 876
    float prone_torso_pitch;	// 880
    int view_locked;			// 884
    int view_locked_ent_num;	// 888

    float friction;				// 892

    int gun_fx;					// 896
    int server_cursor_hint;		// 900
    int server_cursor_hint_value;// 904

    int server_cursor_hint_string; // 908

    int field_912;				// 912
    int field_916;				// 916
    int field_920;				// 920
    int field_924;				// 924
    int field_928;				// 928
    int field_932;				// 932
    int field_936;				// 936
    int field_940;				// 940
    int field_944;				// 944
    int field_948;				// 948
    int field_952;				// 952
    int field_956;				// 956

    int compass_friend_info;	// 960

    float torso_height;			// 964
    float torso_pitch;			// 968
    float waist_pitch;			// 972

    int field_976;				// 976
    int weap_anim;				// 980

    float aim_spread_scale;		// 984

    int shellshock_index;		// 988
    int shellshock_time;		// 992
    int shellshock_duration;	// 996

    char gap[7396];

    int delta_time;				// 8396

    // 1.5 has 3 more fields here
};

#include "surfaceflags.h"

// plane types are used to speed some tests
// 0-2 are axial planes
#define	PLANE_X			0
#define	PLANE_Y			1
#define	PLANE_Z			2
#define	PLANE_NON_AXIAL	3

#define plane_type_for_normal(x) \
    (x[0] == 1.0 ? PLANE_X : \
        (x[1] == 1.0 ? PLANE_Y : \
            (x[2] == 1.0 ? PLANE_Z : PLANE_NON_AXIAL)))

//
// defines here from common.h
//
#ifndef com_printf
void com_printf_runner(enum print_level level, const char *fmt, ...);
#define com_printf(...) com_printf_runner(PRINT_ALL, __VA_ARGS__)
#endif

//
// infostrings
//
char *info_value_for_key(const char *s, const char *key);
void info_next_pair(const char **head, char *key, char *value);
void info_remove_key(char *s, const char *key);
void info_remove_key_big(char *s, const char *key);
boolean info_validate(const char *s);
void info_set_value_for_key(char *s, const char *key, const char *value);
void info_set_value_for_key_big(char *s, const char *key, const char *value);
void info_print(const char *s);

#endif // __SHARED_H__
