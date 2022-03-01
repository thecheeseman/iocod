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
 * @file server_local.h
 * @date 2022-02-04
*/

#ifndef __SERVER_LOCAL_H__
#define __SERVER_LOCAL_H__

#include "shared.h"
#include "common.h"
#include "game.h"

#ifdef CODVERSION1_5
#define PROTOCOL_VERSION 5
#else
#define PROTOCOL_VERSION 1
#endif

#define MAX_MASTER_SERVERS 5
#define MASTER_SERVER_NAME "codmaster.activision.com"

struct shared_entity {
	struct entity_state s;
	struct entity_shared r;
};

/**
 * Not 100% sure if entity_state baseline or shared_entity
*/
struct server_entity {
	struct entity_state baseline;

	char data[36];
};

enum client_state {
	CS_FREE,					/**< can be reused for a new connection */
	CS_ZOMBIE,					/**< client has been disconnected, but don't
									 reuse connection for a couple seconds */
	CS_CONNECTED,				/**< has been assigned to a struct *client,
									 but no gamestate yet */
	CS_PRIMED,					/**< gamestate has been sent, but client hasn't
									 sent a usercmd yet */
	CS_ACTIVE					/**< client is fully in game */
};

/**
 * @brief Client snapshot data
 * 
 * 8428 size
*/
struct client_snapshot {
	struct player_state ps;

	int portals[3];				/**< something to do with portals...
									 could be byte areabits[MAX_MAP_AREA_BYTES] */
	int num_entities;			
	int first_entity;			/**< into the circular sv_packet_entities[]
								     the entities MUST be in increasing state 
								     number order, otherwise the delta 
								     compression will fail */
	int message_sent;			/**< time the message was transmitted */
	int message_acknowledged;	/**< time the message was acknowledged */
	int message_size;			/**< used to rate drop packets */
};

#define MAX_RELIABLE_COMMANDS 64
#define MAX_DOWNLOAD_WINDOW 8
#define PACKET_BACKUP 32

/**
 * @brief Client data
 * 
 * 1.1 size = 0x5a8fc
 * 1.5 size = 0x5a9b4
*/
struct client {
	enum client_state state;	/**< client state */

	int a;
	int b;

	char userinfo[MAX_STRING_CHARS];

	char reliable_commands[MAX_RELIABLE_COMMANDS][1032];
	int reliable_sequence;		/**< last added reliable message,
								     not necessarily sent or acknowledged yet */
	int reliable_acknowledge;	/**< last acknowledged reliable message */
	int reliable_sent;			/**< last sent reliable message, 
								     not necessarily sent or acknowledged yet */
	int message_acknowledge;
	int gamestate_message_num;	/**< netchan->outgoing_sequence of gamestate */

	int challenge;

	struct usercmd last_usercmd;
	int last_client_command;
	int last_client_command_string[MAX_STRING_CHARS];

	struct shared_entity *gentity;

	char name[32];				/**< extracted from userinfo */

	char download_name[64];		/**< if not empty, we are downloading */
	int download;				/**< file being downloaded */
	int download_size;			/**< total bytes (can't use EOF because of paks) */
	int download_count;			/**< bytes sent */
	int download_client_block;	/**< last block we sent to the client that
								     is awaiting ACK */
	int download_current_block;	/**< current block number */
	int download_transmit_block;/**< last block we transmitted */

	byte *download_blocks[MAX_DOWNLOAD_WINDOW];
	int download_block_size[MAX_DOWNLOAD_WINDOW];

	bool download_eof;			/**< we have send the EOF block */
	int download_send_time;		/**< time we last got an ACK from the client */
	int delta_message;			/**< from last client usercmd message */
	int next_reliable_packet;	/**< svs.time when another reliable command
								     will be allowed */
	int last_packet_time;		/**< svs.time when packet was last received */
	int last_connect_time;		/**< svs.time when connection started */
	int next_snapshot_time;		/**< send another snapshot when 
								     svs.time >= next_snapshot_time */
	bool rate_delayed;			/**< true if next_snapshot_time was set based
								     on rate instead of snapshot_msec */
	int timeout_count;			/**< must timeout a few frames in a row
								     so debugging doesn't break */

	struct client_snapshot frames[PACKET_BACKUP];

	int ping;					/**< ping time */
	int rate;					/**< bytes / second */
	int snapshot_msec;			/**< requests a snapshot every snapshot_msec 
								     unless rate choked */
	int pure_authentic;

	struct netchan netchan;

	uint16_t scriptvar;			/**< assigned script variable id */
	uint16_t unreferenced;		

	int netchan_end_queue;

	int net_profiling;			/**< flag if net profiling is enabled */
};

enum server_state {
	SS_DEAD,					/**< no map loaded */
	SS_LOADING,					/**< spawning level entities */
	SS_GAME						/**< actively running */
};

/**
 * @brief Server structure
 * 
 * 1.1 size = 0x614ec
 * 1.5 size = 0x6152c
*/
struct server {
	enum server_state state;

	bool restarting;			/**< if true, send configstring changes
								     during SS_LOADING */
	int server_id;				/**< changes each server start */
	int checksum_feed;			/**< the feed key that we use to compute 
								     the pure checksum strings */
	int time_residual;			/**< <= 1000 / sv_frame->value */
	
	int a;
	
	int models[256];			
	char *configstrings[MAX_CONFIGSTRINGS];

	int b;
	int c;

	struct server_entity sv_entities[MAX_GENTITIES];

	int data[93];

	char *entity_parse_point;	/**< used during game VM init */
	
	struct shared_entity *gentities;
	int gentity_size;
	int num_entities;			/**< current number, <= MAX_GENTITIES */
	struct player_state *game_clients;
	int game_client_size;		/**< will be > sizeof(playerState_t) due to 
								     game private data */

	int d;

	int data2[30];

	#ifdef CODVERSION1_5
	int data3[16];
	#endif

	char gametype[64];			/**< saved gametype string used for handling
								     persistence data */
};

extern struct server sv;

// MAX_CHALLENGES is made large to prevent a denial
// of service attack that could cycle all of them
// out before legitimate users connected
#define MAX_CHALLENGES 1024

/**
 * @brief Challenge for authorize
*/
struct challenge {
	struct netadr adr;

	int challenge;
	int time;					/**< time the last packet was sent to the
								     authorize server */
	int ping_time;				/**< time the challenge reponse was sent to the
								     client */
	int first_time;				/**< time the address was first used
								     for authorize timeout checks */
	int first_ping;
	bool connected;

	#ifdef CODVERSION1_5
	int data[10];
	#endif
};

/**
 * @brief This structure will be cleared when the game dll changes
 * 1.1 size = b084
 * 1.5 size = 1510c
*/
struct server_static {
	bool initialized;			/**< true once sv_init has completed */

	#ifdef CODVERSION1_5
	int time2; // no idea what this is
	#endif
	
	int time;					/**< will be strictly increasing across 
								     level changes */
	int snap_flag_server_bit;	/**< ^= SNAPFLAG_SERVERCOUNT every time
								     sv_spawn_server() is called */

	struct client *clients;

	int num_snapshot_entities;
	int num_snapshot_clients;
	int next_snapshot_entities;
	int next_snapshot_clients;

	struct entitystate *snapshot_entities;
	void *snapshot_clients;

	int field_0x28;

	int next_archived_snapshot_frames;

	int field_0x30;
	int field_0x34;

	int next_archived_snapshot_buffer;
	int next_cached_snapshot_entities;
	int next_cached_snapshot_clients;
	int next_cached_snapshot_frames;

	int field_0x48;
	int field_0x4c;
	int field_0x50;

	int next_heartbeat_time;

	#ifdef CODVERSION1_5
	int field_0x5c;
	#endif

	struct challenge challenges[MAX_CHALLENGES];

	struct netadr redirect_address;
	struct netadr authorize_address;
};

extern struct server_static svs;

extern struct vm *gvm;

extern struct cvar *sv_gametype;
extern struct cvar *sv_mapname;
extern struct cvar *sv_privateclients;
extern struct cvar *sv_hostname;
extern struct cvar *sv_maxclients;
extern struct cvar *sv_maxrate;
extern struct cvar *sv_minping;
extern struct cvar *sv_maxping;
extern struct cvar *sv_floodprotect;
extern struct cvar *sv_allowanonymous;
extern struct cvar *sv_showcommands;
extern struct cvar *sv_serverid;
extern struct cvar *sv_pure;
extern struct cvar *sv_rconpassword;
extern struct cvar *sv_privatepassword;
extern struct cvar *sv_fps;
extern struct cvar *sv_timeout;
extern struct cvar *sv_zombietime;
extern struct cvar *sv_allowdownload;
extern struct cvar *sv_master[MAX_MASTER_SERVERS];
extern struct cvar *sv_reconnectlimit;
extern struct cvar *sv_showloss;
extern struct cvar *sv_padpackets;
extern struct cvar *sv_killserver;
extern struct cvar *sv_onlyvisibleclients;
extern struct cvar *sv_showaveragebps;
extern struct cvar *sv_maprotation;
extern struct cvar *sv_maprotation_current;

#ifdef CODVERSION1_5
extern struct cvar *sv_punkbuster;
extern struct cvar *sv_disableclientconsole;
extern struct cvar *sv_packet_info;
extern struct cvar *sv_kickbantime;
extern struct cvar *sv_wwwdownload;
extern struct cvar *sv_wwwbaseurl;
extern struct cvar *sv_wwwdldisconnected;
#endif

//
//
// sv_cmds.c
//
//
void sv_heartbeat_f(void);
void sv_add_operator_commands(void);

//
//
// server_game.c
//
//
struct shared_entity *sv_gentity_num(int num);

//
//
// server_init.c
//
//
void sv_set_gametype(void);
void sv_reset_snapshot_data(void);
void sv_set_config_string(int index, const char *val);
void sv_get_config_string(int index, char *buffer, int bufsize);

//
//
// server_main.c
//
//
void sv_set_cvar_config_string(int index, int count, int bit);
void sv_game_run_frame(void);
void sv_send_server_command(struct client *cl, const char *fmt, ...);

#endif // __SERVER_LOCAL_H__
