// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_GAME_H
#define GAME_GAME_H

typedef struct GameImports GameImports;
typedef struct GameExports GameExports;

/// @brief Game entry point.
/// @param[in] imports Game imports from system
/// @param[out] exports Game exports to system
typedef void (*GameEntryPointPtr)(GameImports*, GameExports*);

/// Functions imported from the system to the game dll
struct GameImports {
    /// size of this structure
    int size;

    /// print message on the local console
    void (*print)(const char* string);

    /// abort the game
    void (*error)(const char* string);

    /// localized error message
    void (*error_localized)(const char* string);

    /// get current time
    int (*milliseconds)(void);

    /*
        //============== general Quake services ==================

    G_PRINT, // (const char *string);
    // print message on the local console

    G_ERROR, // (const char *string);
    // abort the game

    //! added in 1.1
    G_ERROR_LOCALIZED, // (const char *string);
    //! added in 1.1

    G_MILLISECONDS, // (void);
    // get current time for profiling reasons
    // this should NOT be used for any game related tasks,
    // because it is not journaled

    // console variable interaction
    G_CVAR_REGISTER, // (vmCvar_t *vmCvar, const char *varName, const char *defaultValue);
    G_CVAR_UPDATE,   // (vmCvar_t *vmCvar);
    G_CVAR_SET,      // (const char *var_name, const char *value);
    G_CVAR_VARIABLE_INTEGER_VALUE, // (const char *var_name);

    //! added in 1.1
    G_CVAR_VARIABLE_VALUE, // (const char *var_name);
    //! added in 1.1

    G_CVAR_VARIABLE_STRING_BUFFER, // (const char *var_name, char *buffer, int bufsize);

    // 10
    G_ARGC, // (void);
    // ClientCommand and ServerCommand parameter access

    G_ARGV, // (int n, char *buffer, int bufferLength);

    //! added in 1.1
    G_HUNK_ALLOC_INTERNAL,                // (int amount);
    G_HUNK_ALLOC_LOW_INTERNAL,            // (int amount);
    G_HUNK_ALLOC_ALIGN_INTERNAL,          // (int amount, int align);
    G_HUNK_ALLOC_LOW_ALIGN_INTERNAL,      // (int amount, int align);
    G_HUNK_ALLOCATE_TEMP_MEMORY_INTERNAL, // (int amount);
    G_HUNK_FREE_TEMP_MEMORY_INTERNAL,     // (int amount);
    //! added in 1.1

    G_FS_FOPEN_FILE, // (const char *qpath, fileHandle_t *file, fsMode_t mode);
    G_FS_READ,       // (void *buffer, int len, fileHandle_t f);

    // 20
    G_FS_WRITE,       // (const void *buffer, int len, fileHandle_t f);
    G_FS_RENAME,      // (const char *from, const char *to);
    G_FS_FCLOSE_FILE, // (fileHandle_t f);

    G_SEND_CONSOLE_COMMAND, // (const char *text);
    // add commands to the console as if they were typed in
    // for map changing, etc

    //=========== server specific functionality =============

    G_LOCATE_GAME_DATA, // (gentity_t *gEnts, int numGEntities, int sizeofGEntity_t);
    // the game needs to let the server system know where and how big the gentities
    // are, so it can look at them directly without going through an interface

    G_DROP_CLIENT, // (int clientNum, const char *reason);
    // kick a client off the server with a message

    G_SEND_SERVER_COMMAND, // (int clientNum, int other, const char *text);
    // reliably sends a command string to be interpreted by the given
    // client.  If clientNum is -1, it will be sent to all clients

    G_SET_CONFIG_STRING, // (int num, const char *string);
    // config strings hold all the index strings, and various other information
    // that is reliably communicated to all clients
    // All of the current configstrings are sent to clients when
    // they connect, and changes are sent to all connected clients.
    // All confgstrings are cleared at each level start.

    G_GET_CONFIG_STRING, // (int num, char *buffer, int bufferSize);

    //! added in 1.1
    G_GET_CONFIG_STRING_CONST, // (int num)
    //! added in 1.1

    // 30
    G_GET_USERINFO, // (int num, char *buffer, int bufferSize);
    // userinfo strings are maintained by the server system, so they
    // are persistant across level loads, while all other game visible
    // data is completely reset

    G_SET_USERINFO, // (int num, const char *buffer);

    G_GET_SERVERINFO, // (char *buffer, int bufferSize);
    // the serverinfo info string has all the cvars visible to server browsers

    G_SET_BRUSH_MODEL, // (gentity_t *ent);
    // sets mins and maxs based on the brushmodel name

    G_TRACE, // (trace_t *results, const vec3_t start, const vec3_t end);
    // collision detection against all linked entities

    //! added in 1.1
    G_TRACE_CAPSULE,         // (trace_t *results, const vec3_t start, const vec3_t end);
    G_SIGHT_TRACE,           // (trace_t *results, const vec3_t start, const vec3_t end);
    G_SIGHT_TRACE_CAPSULE,   // (trace_t *results, const vec3_t start, const vec3_t end);
    G_SIGHT_TRACE_TO_ENTITY, // (trace_t *results, const vec3_t start, const vec3_t end);

    G_CM_BOX_TRACE, // (trace_t *results, const vec3_t start, const vec3_t end);

    // 40
    G_CM_CAPSULE_TRACE,       // (trace_t *results, const vec3_t start, const vec3_t end);
    G_CM_BOX_SIGHT_TRACE,     // (trace_t *results, const vec3_t start, const vec3_t end);
    G_CM_CAPSULE_SIGHT_TRACE, // (trace_t *results, const vec3_t start, const vec3_t end);

    G_LOCATIONAL_TRACE, // (trace_t *results, const vec3_t start, const vec3_t end);
    //! added in 1.1

    G_POINT_CONTENTS, // (const vec3_t point, int passEntityNum, int contentsMask);
    // point contents against all linked entities

    G_IN_PVS, // (const vec3_t p1, const vec3_t p2);

    G_IN_PVS_IGNORE_PORTALS, // (const vec3_t p1, const vec3_t p2);

    //! added in 1.1
    G_IN_SNAPSHOT, // (const vec3_t p1, gentity_t *ent);
    //! added in 1.1

    G_ADJUST_AREA_PORTAL_STATE, // (gentity_t *ent, qboolean open);

    G_AREAS_CONNECTED, // (int area1, int area2);

    // 50
    G_LINKENTITY, // (gentity_t *ent);
    // an entity will never be sent to a client or used for collision
    // if it is not passed to linkentity.  If the size, position, or
    // solidity changes, it must be relinked.

    G_UNLINKENTITY, // (gentity_t *ent);
    // call before removing an interactive entity

    G_ENTITIES_IN_BOX, // (const vec3_t mins, const vec3_t maxs, int *list);
    // EntitiesInBox will return brush models based on their bounding box,
    // so exact determination must still be done with EntityContact

    G_ENTITY_CONTACT, // (const vec3_t mins, const vec3_t maxs, const gentity_t *ent);
    // perform an exact check against inline brush models of non-square shape

    G_GET_USERCMD, // (int clientNum, usercmd_t *cmd);

    G_GET_ENTITY_TOKEN, // (char *buffer, int bufferSize);
    // Retrieves the next string token from the entity spawn text, returning
    // false when all tokens have been parsed.
    // This should only be done at GAME_INIT time.

    G_FS_GET_FILE_LIST, // (const char *path, const char *extension, char *listbuf);
    G_REAL_TIME,        // (qtime_t *qtime);
    G_SNAPVECTOR,       // (float *v);

    G_ENTITY_CONTACTCAPSULE, // (const vec3_t mins, const vec3_t maxs, const gentity_t *ent);
    // perform an exact check against inline brush models of non-square shape

    //! added in 1.1
    // 60
    G_COM_SOUNDALIAS_STRING,  // (int a1);
    G_COM_PICK_SOUNDALIAS,    // (int a1);
    G_COM_SOUNDALIAS_INDEX,   // (int a1);
    G_SURFACE_TYPE_FROM_NAME, // (int a1);
    G_SURFACE_TYPE_TO_NAME,   // (int a1);

    G_ADD_TEST_CLIENT, // (void);

    G_GET_ARCHIVED_PLAYERSTATE, // (int spectatorClient, int archiveTime, char *buf);

    G_ADD_DEBUG_STRING, // (const vec3_t origin, float size, char *str);
    G_ADD_DEBUG_LINE,   // (const vec3_t start, const vec3_t end, const vec3_t color);

    G_SET_ARCHIVE, // (qboolean archived);

    G_Z_MALLOC_INTERNAL, // (int a1);
    G_Z_FREE_INTERNAL,   // (int a1);

    G_XANIM_CREATE_TREE, G_XANIM_CREATE_SMALL_TREE, G_XANIM_FREE_SMALL_TREE,

    G_XMODEL_EXISTS, G_XMODEL_GET,

    G_DOBJ_CREATE, G_DOBJ_EXISTS, G_SAFE_DOBJ_FREE,

    // duplicates with different results
    G_XANIM_GET_ANIMS,
    G_XANIM_GET_ROOT, // 80 technically

    G_XANIM_CLEAR_TREE_GOAL_WEIGHTS, G_XANIM_CLEAR_GOAL_WEIGHT,
    G_XANIM_CLEAR_TREE_GOAL_WEIGHTS_STRICT, G_XANIM_SET_COMPLETE_GOAL_WEIGHT_KNOB,
    G_XANIM_SET_COMPLETE_GOAL_WEIGHT_KNOB_ALL, G_XANIM_SET_ANIM_RATE, G_XANIM_SET_TIME,
    G_XANIM_SET_GOAL_WEIGHT_KNOB, G_XANIM_CLEAR_TREE, G_XANIM_HAS_TIME, G_XANIM_IS_PRIMITIVE,
    G_XANIM_GET_LENGTH, G_XANIM_GET_LENGTH_SECONDS, G_XANIM_SET_COMPLETE_GOAL_WEIGHT,
    G_XANIM_SET_GOAL_WEIGHT, G_XANIM_CALC_ABS_DELTA, G_XANIM_CALC_DELTA, G_XANIM_GET_REL_DELTA,
    G_XANIM_GET_ABS_DELTA, G_XANIM_IS_LOOPED, G_XANIM_NOTETRACK_EXISTS, G_XANIM_GET_TIME,
    G_XANIM_GET_WEIGHT,

    G_DOBJ_DUMP_INFO, G_DOBJ_CREATE_SKEL_FOR_BONE, G_DOBJ_CREATE_SKEL_FOR_BONES,
    G_DOBJ_UPDATE_SERVER_TIME, G_DOBJ_INIT_SERVER_TIME, G_DOBJ_GET_HIERARCHY_BITS,
    G_DOBJ_CALC_ANIM, G_DOBJ_CALC_SKEL,

    G_XANIM_LOAD_ANIM_TREE, G_XANIM_SAVE_ANIM_TREE, G_XANIM_CLONE_ANIM_TREE,

    G_DOBJ_NUM_BONES, G_DOBJ_GET_BONE_INDEX, G_DOBJ_GET_MATRIX_ARRAY, G_DOBJ_DISPLAY_ANIM,

    G_XANIM_HAS_FINISHED, G_XANIM_GET_NUM_CHILDREN, G_XANIM_GET_CHILD_AT,

    G_XMODEL_GET_NUM_BONES, G_XMODEL_GET_BONE_NAMES,

    G_DOBJ_GET_ROT_TRANS_ARRAY, G_DOBJ_SET_ROT_TRANS_INDEX, G_DOBJ_SET_CONTROL_ROT_TRANS_INDEX,

    G_XANIM_GET_ANIM_NAME,

    G_DOBJ_GET_TREE,

    G_XANIM_GET_ANIM_TREE_SIZE,

    G_XMODEL_DEBUG_BOXES,

    G_GET_WEAPON_INFO_MEMORY, G_FREE_WEAPON_INFO_MEMORY,

    G_FREE_CLIENT_SCRIPT_PERS, G_RESET_ENTITY_PARSE_POINT
    //! added in 1.1
    */
};

/// Functions exported by the game dll
struct GameExports {
    /// size of this struct
    /// if size == -1 then an error occurred or the system version is too old
    int size;

    /// 
    void (*init)(int level_time, int random_seed, int restart, int);

    // int (*version)(void); // unnecessary

    void (*shutdown)(void);

    /*
        GAME_INIT, // (int leveltime, int randomseed, int restart, int a);
            // init and shutdown will be called every single level

            #ifdef CODVERSION1_5
            GAME_VERSION, // (void);
            // return an integer version (5)

            GAME_IDK,
            #endif

            GAME_SHUTDOWN, // (void);
            // init and shutdown will be called every single level

            GAME_CLIENT_CONNECT,

            GAME_CLIENT_BEGIN,

            GAME_CLIENT_USERINFO_CHANGED,

            GAME_CLIENT_DISCONNECT,

            GAME_CLIENT_COMMAND,

            GAME_CLIENT_THINK,

            GAME_GET_FOLLOW_PLAYERSTATE,

            GAME_UPDATE_CVARS,

            GAME_RUN_FRAME,

            GAME_CONSOLE_COMMAND,

            GAME_SCRIPT_FARHOOK,

            GAME_CALC_POSE,

            GAME_VALID_WEAPON,

            GAME_SET_PERSISTENT,

            GAME_GET_PERSISTENT,

            GAME_GET_CLIENT_2178,

            GAME_GET_CLIENT_ARCHIVETIME,

            GAME_SET_CLIENT_ARCHIVETIME,

            GAME_GET_CLIENT_SCORE
    */
};

#endif // GAME_GAME_H
