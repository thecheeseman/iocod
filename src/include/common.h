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
 * @file common.h
 * @date 2022-02-04
*/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdarg.h>

#define MAX_MSGLEN 16384

enum system_event_type {
	SE_NONE = 0,
	SE_KEY,
	SE_CHAR,
	SE_MOUSE,
	SE_JOYSTICK_AXIS,
	SE_CONSOLE,
	SE_PACKET,
	SE_BAD_EVENT
};

struct system_event {
	int time;
	enum system_event_type type;
	int	value;
	int value2;
	int	ptr_length; // bytes of data pointed to by ptr, for journaling
	void *ptr;		// this must be manually freed if not NULL
};

#define	MAX_EDIT_LINE	256
struct field {
	int		cursor;
	int		scroll;
	int		widthInChars;
	char	buffer[MAX_EDIT_LINE];
};

// ZONE NONSENSE
enum {
	TAG_FREE,
	TAG_GENERAL,
	TAG_BOTLIB,
	TAG_RENDERER,
	TAG_SMALL,
	TAG_STATIC
};
// ZONE NONSENSE

//
//
// global definitions are located in the global headers 
// for each corresponding module
//
//
#include "global/cmd.h"
#include "global/common.h"
#include "global/files.h"
#include "global/net.h"
#include "global/null_client.h"
#include "global/server.h"
#include "global/script.h"
#include "global/system.h"

//
//
// md4.c
//
//
unsigned com_block_checksum(const void *buffer, int length);
unsigned com_block_checksum_key(void *buffer, int length, int key);

//
//
// network/msg.c
//
//
void msg_init(struct msg *buf, byte *data, int length);

//
//
// network/net.c
//
//
void net_init(void);
void net_shutdown(void);
void net_open_ip(void);

//
//
// network/net_chan.c
//
//
void netchan_init(int port);
bool net_get_loop_packet(int sock, struct netadr *from, struct msg *msg);

//
//
// script/scr_init.c
//
//
//void scr_init(int developer, int developer_script);
//void scr_abort(void);
//void scr_shutdown(void);

//
//
// vm.c
//
//
struct vm {
	intptr_t (*system_call)(intptr_t *parms);

	char name[MAX_QPATH];
	char full_qpath[MAX_QPATH + 1];

	char data[3];

	void *dll_handle;
	intptr_t (*entry_point)(intptr_t callnum, ...);
};

void vm_init(void);
struct vm *vm_create(const char *module, intptr_t (*systemcalls)(intptr_t *));
void vm_free(struct vm *vm);
void vm_clear(void);
intptr_t QDECL vm_call(struct vm *vm, intptr_t callnum, ...);

#endif // __COMMON_H__
