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
 * @file server_game.c
 * @date 2022-02-04
*/

#include "server_local.h"

struct shared_entity *sv_gentity_num(int num)
{
	struct shared_entity *ent;

	ent = (struct shared_entity *) ((byte *) sv.gentities + sv.gentity_size * (num));

	return ent;
}

#define VMA(x) ((void *) args[x])
#define VMF(x) ((float *) args)[x]

intptr_t sv_game_systemcalls(intptr_t *args)
{
	switch (args[0]) {
		case G_PRINT:
			com_printf("%s", (char *) args[1]);
			break;
		case G_ERROR:
			com_error(ERR_DROP, "%s", (char *) args[1]);
			break;
		case G_ERROR_LOCALIZED:
			break;
		case G_MILLISECONDS:
			return sys_milliseconds();
		case G_CVAR_REGISTER:
		case G_CVAR_UPDATE:
		case G_CVAR_SET:
		case G_CVAR_VARIABLE_INTEGER_VALUE:
		case G_CVAR_VARIABLE_VALUE:
		case G_CVAR_VARIABLE_STRING_BUFFER:
			break;
		case G_ARGC:
			return cmd_argc();
		case G_ARGV:
			cmd_argv_buffer(args[1], (char *) args[2], args[3]);
			return 0;
		case G_HUNK_ALLOC_INTERNAL:
		case G_HUNK_ALLOC_LOW_INTERNAL:
		case G_HUNK_ALLOC_ALIGN_INTERNAL:
		case G_HUNK_ALLOC_LOW_ALIGN_INTERNAL:
		case G_HUNK_ALLOCATE_TEMP_MEMORY_INTERNAL:
		case G_HUNK_FREE_TEMP_MEMORY_INTERNAL:
		case G_FS_FOPEN_FILE:
		case G_FS_WRITE:
		case G_FS_RENAME:
		case G_FS_FCLOSE_FILE:
		case G_SEND_CONSOLE_COMMAND:
		case G_LOCATE_GAME_DATA:
		case G_DROP_CLIENT:
		case G_SEND_SERVER_COMMAND:
		case G_SET_CONFIG_STRING:
		case G_GET_CONFIG_STRING:
		case G_GET_CONFIG_STRING_CONST:
		case G_GET_USERINFO:
		case G_SET_USERINFO:
		case G_GET_SERVERINFO:
		case G_SET_BRUSH_MODEL:
		case G_TRACE:
		case G_TRACE_CAPSULE:
		case G_SIGHT_TRACE:
		case G_SIGHT_TRACE_CAPSULE:
		case G_SIGHT_TRACE_TO_ENTITY:
		case G_CM_BOX_TRACE:
		case G_CM_CAPSULE_TRACE:
		case G_CM_BOX_SIGHT_TRACE:
		case G_CM_CAPSULE_SIGHT_TRACE:
		case G_LOCATIONAL_TRACE:
		case G_POINT_CONTENTS:
		case G_IN_PVS:
		case G_IN_PVS_IGNORE_PORTALS:
		case G_IN_SNAPSHOT:
		case G_ADJUST_AREA_PORTAL_STATE:
		case G_AREAS_CONNECTED:
		case G_LINKENTITY:
		case G_UNLINKENTITY:
		case G_ENTITIES_IN_BOX:
		case G_ENTITY_CONTACT:
		case G_GET_USERCMD:
		case G_GET_ENTITY_TOKEN:
		case G_FS_GET_FILE_LIST:
		case G_REAL_TIME:
		case G_SNAPVECTOR:
		case G_ENTITY_CONTACTCAPSULE:
		case G_COM_SOUNDALIAS_STRING:
		case G_COM_PICK_SOUNDALIAS:
		case G_COM_SOUNDALIAS_INDEX:
		case G_SURFACE_TYPE_FROM_NAME:
		case G_SURFACE_TYPE_TO_NAME:
		case G_ADD_TEST_CLIENT:
		case G_GET_ARCHIVED_PLAYERSTATE:
		case G_ADD_DEBUG_STRING:
		case G_ADD_DEBUG_LINE:
		case G_SET_ARCHIVE:
		case G_Z_MALLOC_INTERNAL:
		case G_Z_FREE_INTERNAL:
		case G_XANIM_CREATE_TREE:
		case G_XANIM_CREATE_SMALL_TREE:
		case G_XANIM_FREE_SMALL_TREE:
		case G_XMODEL_EXISTS:
		case G_XMODEL_GET:
		case G_DOBJ_CREATE:
		case G_DOBJ_EXISTS:
		case G_SAFE_DOBJ_FREE:
		case G_XANIM_GET_ANIMS:
		case G_XANIM_GET_ROOT:
		case G_XANIM_CLEAR_TREE_GOAL_WEIGHTS:
		case G_XANIM_CLEAR_GOAL_WEIGHT:
		case G_XANIM_CLEAR_TREE_GOAL_WEIGHTS_STRICT:
		case G_XANIM_SET_COMPLETE_GOAL_WEIGHT_KNOB:
		case G_XANIM_SET_COMPLETE_GOAL_WEIGHT_KNOB_ALL:
		case G_XANIM_SET_ANIM_RATE:
		case G_XANIM_SET_TIME:
		case G_XANIM_SET_GOAL_WEIGHT_KNOB:
		case G_XANIM_CLEAR_TREE:
		case G_XANIM_HAS_TIME:
		case G_XANIM_IS_PRIMITIVE:
		case G_XANIM_GET_LENGTH:
		case G_XANIM_GET_LENGTH_SECONDS:
		case G_XANIM_SET_COMPLETE_GOAL_WEIGHT:
		case G_XANIM_SET_GOAL_WEIGHT:
		case G_XANIM_CALC_ABS_DELTA:
		case G_XANIM_CALC_DELTA:
		case G_XANIM_GET_REL_DELTA:
		case G_XANIM_GET_ABS_DELTA:
		case G_XANIM_IS_LOOPED:
		case G_XANIM_NOTETRACK_EXISTS:
		case G_XANIM_GET_TIME:
		case G_XANIM_GET_WEIGHT:
		case G_DOBJ_DUMP_INFO:
		case G_DOBJ_CREATE_SKEL_FOR_BONE:
		case G_DOBJ_CREATE_SKEL_FOR_BONES:
		case G_DOBJ_UPDATE_SERVER_TIME:
		case G_DOBJ_INIT_SERVER_TIME:
		case G_DOBJ_GET_HIERARCHY_BITS:
		case G_DOBJ_CALC_ANIM:
		case G_DOBJ_CALC_SKEL:
		case G_XANIM_LOAD_ANIM_TREE:
		case G_XANIM_SAVE_ANIM_TREE:
		case G_XANIM_CLONE_ANIM_TREE:
		case G_DOBJ_NUM_BONES:
		case G_DOBJ_GET_BONE_INDEX:
		case G_DOBJ_GET_MATRIX_ARRAY:
		case G_DOBJ_DISPLAY_ANIM:
		case G_XANIM_HAS_FINISHED:
		case G_XANIM_GET_NUM_CHILDREN:
		case G_XANIM_GET_CHILD_AT:
		case G_XMODEL_GET_NUM_BONES:
		case G_XMODEL_GET_BONE_NAMES:
		case G_DOBJ_GET_ROT_TRANS_ARRAY:
		case G_DOBJ_SET_ROT_TRANS_INDEX:
		case G_DOBJ_SET_CONTROL_ROT_TRANS_INDEX:
		case G_XANIM_GET_ANIM_NAME:
		case G_DOBJ_GET_TREE:
		case G_XANIM_GET_ANIM_TREE_SIZE:
		case G_XMODEL_DEBUG_BOXES:
		case G_GET_WEAPON_INFO_MEMORY:
		case G_FREE_WEAPON_INFO_MEMORY:
		case G_FREE_CLIENT_SCRIPT_PERS:
		case G_RESET_ENTITY_PARSE_POINT:
			break;
		
		default:
			com_error(ERR_DROP, "Bad game system trap: %i", args[0]);
	}

	return 0;
}

static void sv_init_game_vm(bool restart, bool keep_persistent)
{
	// sv.entityparsepoint = cm_entity_string();

	// ret = vm_call(gvm, GAME_SCRIPT_FARHOOK, scr_far_hook(NULL));
	// scr_far_hook(ret);

	#ifdef CODVERSION1_5
	vm_call(gvm, GAME_INIT, svs.time2, com_milliseconds(), restart, 
			keep_persistent);
	#else
	vm_call(gvm, GAME_INIT, svs.time, com_milliseconds(), restart, 
			keep_persistent);
	#endif

	// for (i = 0; i < sv_maxclients->integer; i++)
	//	svs.clients[i].gentity = NULL;

	// if (com_dedicated->integer)
	//	cvar_dump(PRINT_LOGONLY);
}

void sv_init_game_progs(bool keep_persistent)
{
	gvm = vm_create("game", sv_game_systemcalls);
	if (!gvm)
		com_error(ERR_FATAL, "vm_create on game failed");

	hunk_set_mark_low();

	#ifdef CODVERSION1_5
	int version = vm_call(gvm, GAME_VERSION);
	if (version != 5)
		com_error(ERR_FATAL, "game version is %d, expected %d", version, 5);
	#endif

	sv_init_game_vm(false, keep_persistent);
}

void sv_shutdown_game_progs(void)
{
	if (!gvm)
		return;

	// fun_080bfea0(1);
	vm_call(gvm, GAME_SHUTDOWN, false);

	hunk_clear_to_mark_low();

	vm_free(gvm);
	gvm = NULL;

	// fun_080bfea0(2);
}

bool sv_game_command(void)
{
	if (sv.state != SS_GAME)
		return false;

	// fun_080cb8ee
	// DAT_0832d040 = 1

	return vm_call(gvm, GAME_CONSOLE_COMMAND);
}
