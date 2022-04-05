TODO
----
- update MAX_QPATH to something modern (like 256)
- figure out how to parse terrain data
- general naming conventions for folders and files
    - remove any prefixes e.g. `common_` or `server_`
    - just simplify naming so that each file is named what it needs to be
    - shorter names are preferred
- game dll
    - as above, separate into more folders to make it cleaner
    - scripts
        - module support?
- implement OOB / networking so server can at least send/receive packets
- files_io:372 // TODO: whatever this is // fun_080612a2(*file);
- modularize:
    - files
    - common stuff
    - system stuff

### macos specific
- fix: 'syscall' is deprecated
- fix: format string is not a string literal printf stuff

- <stddef.h> contains NULL & size_t
- <stdint.h> contains intX_t, uintX_t, intptr_t, uintptr_t etc

- MACOS_BUNDLE cmake flag
- MACOS_BUNDLE_INFO_PLIST stuff

### other ideas
- refactor everything
    - really just try and eliminate reliance on `shared.h`
    - only include shared.h when you need `va()` etc
        - maybe even split out shared functions into their own modules
        - like string stuff -> `stringlib.h`
        - infostrings -> `infostrings.h`
        - etc.
    - consider using `-` instead of `_` for filename separators
    - also figure out how to eliminate as many globals as possible
        - think about using structs instead of globals
        - what data needs to be global?
- style
    - think about using windows / gnome style for function declarations
        - e.g. 
            ```
            void
            function(int arg1,
                     int arg2)
            ```
        - instead of 
            ```
            void function(int arg1, int arg2);
            ```
    - windows suggestion to use one line per variable definition
        - e.g.
            ```
            int a; 
            int b; 
            int c;
            ```
        - instead of 
            ```
            int a, b, c;
            ```
    - consider whether to use windows style `_In_` or `_In_Out_` for 
    function parameters (or better styled) like `IN` or `OUT`
 

Incomplete Functions
--------------------

List of functions which are in varying degrees of completion

src/clipmap/cm_load.c
- `cm_load_map`

src/clipmap/cm_patch.c
- `cm_generate_terrain_collide`
- `cm_set_border_inward`
- `cm_terrain_func1`

src/common/common.c
- `com_append_cdkey`
- `com_error_handler`
- `com_error_runner`
- `com_init`
- `com_read_cdkey`
- `com_set_recommended`
- `com_shutdown`
- `com_write_defaults_to_file`

src/common/common_memory.c  
- `fun_08061888`
- `fun_08061980`
- `fun_080c27ec`

src/cvar/cvar_cmds.c  
- `cvar_dump_f`

src/files/files.c  
- `fun_08061070`
- `fs_pure_server_set_loaded_paks`
- `fs_shutdown`

src/game/game_hud.c
- `hudelem_destroy_all`

src/game/game_main.c  
- `g_init_game`
- `g_run_frame`
- `g_shutdown_game`
- `vmMain`

src/game/game_misc.c
- `model_index`

src/game/game_svcmds.c  
- `svcmd_entity_list`

src/network/msg.c  
- `huff_decompress`
- `msg_begin_reading_oob`
- `msg_init`
- `msg_read_string_line`

src/server/server_cmds.c  
- `sv_banclient_f`
- `sv_banuser_f`
- `sv_clientkick_f`
- `sv_dumpuser_f`
- `sv_gamecompletestatus_f`
- `sv_kick_f`
- `sv_killserver_f`
- `sv_map_restart_f`
- `sv_maprotate_f`
- `sv_scriptusage_f`
- `sv_set_drawfriend_f`
- `sv_set_friendlyfire_f`
- `sv_set_killcam_f`
- `sv_status_f`
- `sv_stringusage_f`
- `sv_tell_f`
- `sv_tempbanclient_f`
- `sv_tempbanuser_f`
- `sv_unbanuser_f`

src/server/server_game.c
- `sv_init_game_vm`
- `sv_shutdown_game_progs`
- `sv_game_command`
- `sv_game_systemcalls`

src/server/server_init.c  
- `sv_spawn_server`
- `sv_shutdown`

src/server/server_main.c
- `sv_set_cvar_config_string`
- `sv_calculate_pings`
- `sv_check_timeouts`
- `sv_send_client_messages`
- `sv_master_heartbeat`
- `sv_bot_frame`
- `sv_frame`
- `sv_add_server_command`
- `svc_get_status`
- `svc_get_info`
- `sv_get_challenge`
- `sv_direct_connect`
- `sv_authorize_ip_packet`
- `sv_remote_command`
- `sv_connectionless_packet`
- `sv_packet_event`

src/sound/soundalias.c  
- `snd_parse_alias_file`
- `snd_validate_token`
