# TODO

## Current

## General

- names for OS
    - MacOS -> `Darwin`
    - Linux -> `Linux`
    - Windows -> `Windows`
- names for various build architectures
    - ARM
        - `arm32`
        - `arm64`
    - x86_64
        - `i386`
        - `amd64`
- filenames
    - iocod binary
        - `iocod32`
        - `iocod64`
    - library objects
        - `icgame32`
        - `icgame64`
- build platform info examples
    - `iocod-linux-i386`
    - `iocod-windows-amd64`
    - `iocod-macos-arm64`
- release filenames
    - Linux
        - `iocod-0.30.0-linux-i386.tgz`
        - `iocod-0.30.0-linux-amd64.tgz`
        - `iocod-0.30.0-linux-arm32.tgz`
    - MacOS
        - `iocod-0.30.0-darwin-amd64.tgz`
        - `iocod-0.30.0-darwin-arm64.tgz`
    - Windows
        - `iocod-0.30.0-win32.zip`
        - `iocod-0.30.0-win64.zip`

### Module system

- callbacks code:
    - have `add_callback(struct m_module *m, struct m_callback *);`
    - so users create a local copy and then that is copied over to the internal
      array
    - easier to do this way instead of having a huge function stack
    - enums for callback types ??
- allow dynamic libraries to be loaded at runtime which can extend the 
  functionality of the program
- modules should have `callbacks` or `hooks` into the real code, e.g.
    - `sv_frame`
    - `com_frame`
    - `com_init`
    - etc.
- modules should not have to be linked against anything
    - we provide functionality via `extern'd` functions in the headers
    - they will be satisfied on runtime
- FUTURE: module hooks for scripts + adding script functionality

### File system

- files_io:372 // TODO: whatever this is // fun_080612a2(*file);
- consider automagically converting base pk3s -> one pak zlib or something

### BSP

- figure out how to parse terrain data

### Networking

- implement OOB / networking so server can at least send/receive packets

### Script system

In terms of importance, ideas are rated [1] low importance - [5] high importance
- preprocessor stuff
    - [4] `#include`
        ```c
        /* cod2+ style */
        #include maps\mp\_utility;
        ```
    - [5] `#define` 
        ```c
        #define CONST_INT       1
        #define CONST_FLOAT     3.14
        #define CONST_STRING    "string"

        /* e.g. this code */
        val = 52 / CONST_FLOAT;
        /* becomes this after processing */
        val = 52 / 3.14;
        ```
    - [1] #if, #ifdef, #ifndef, #elif, #else, #endif
        ```c
        #ifndef SOME_VALUE
        #define SOME_VALUE 55
        #endif

        /* allow comparison operators: == != >= <= > < */
        #if ANOTHER_VALUE >= SOME_VALUE
        /* ... */
        #elif ANOTHER_VALUE <= SOME_VALUE && ANOTHER_VALUE > 0
        /* ... */
        #else
        /* ... */
        #endif
        ```
    - [1] support for operators
        ```c
        && || ! 
        > < >= <= == !=
        ```
- [3] `foreach() `
    ```c
    players = getEntArray("player", "classname");
    foreach (player in players) {
        /* do something */
    }
    ```
- consts?
    - other option instead of implementing `#define` is to do
      global consts like MW2+
        ```c
        C_VALUE = 45;

        some_function() {
            /* ... */
            v = C_VALUE + 1;
        }
        ```
    - another idea is to allow enums
        ```c
        const {
            V_ONE = 1,
            V_TWO,
            V_THREE
        };
        ```
- [2] `do { ... } while()`
- increase stack size > 65536 WE HAVE THE TECHNOLOGY
- 64 bit integers
- [3] string interpolation
    ```c
    print(player.name + " killed " + player2.name + " with a " + weapon);
    // is equivalent to
    print("${player.name} killed ${player2.name} with a ${weapon}");
    ```
- [4] gsx format
    - for maximum compatiblity we can create .gsx files
    - interchangeable with .gsc
    - game looks for .gsx files _first_ before falling back to .gsc
    - allows compatibility with vanilla installs without breaking

## Other ideas / suggestions
- general naming conventions for folders and files
    - remove any prefixes e.g. `common_` or `server_`
    - just simplify naming so that each file is named what it needs to be
    - shorter names are preferred
- game dll
    - as above, separate into more folders to make it cleaner
    - scripts
        - module support?
- modularize:
    - files
    - common stuff
    - system stuff
- prawy suggestions
    - Ah for client also weapons file settings needs to be fixed. 
      Like if you use smoke grenade slot weird stuff happens and clip only 
      is bugged i believe
    - for unknown reasons if weapon has 0 ammo, model isn't appearing but 
      if u use cliponly 1 it is" that's what I found out sometime ago
    - Idea for making 1.1 like 1.51. with more weapons, more weapons 
      settings, more weapons slots, vehicles just like original UO sounds 
      very good
    - make vsay command work like in CoD UO 1.5 example /bind "key vsay 1 1" 
      so output is Follow me! They fixed vsay on 1.51
    - idea: fix for silence footsteps while on 500 fps or higher 
    - idea: fix connection interrupted while on high fps
- fix pings on scoreboard
- max gametypes 32 -> 64 ?
- server name more than 32 chars?
- increase max models 256 -> 1024 
- versioning stuff
    - autodetect version at run time
    - only load specific paks for 1.5 when needed, etc
    - config file for "default" cod version
- client
    - replace pb field in server browser with version field?
    - or add custom version field
    - language var or something to force loading from localized_lang_pak
      fix bug where codmp.exe won't load polish lang font
- uo compat or at least features?
    - vehicles
    - sprinting
    - maybe future just have a iocod project that can join
      all cod1/uo servers ?
- codextended compatibility
    - script compat is most important
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

## Fixes

### macos
- fix: 'syscall' is deprecated
- fix: format string is not a string literal printf stuff

- <stddef.h> contains NULL & size_t
- <stdint.h> contains intX_t, uintX_t, intptr_t, uintptr_t etc

- MACOS_BUNDLE cmake flag
- MACOS_BUNDLE_INFO_PLIST stuff

## NOTES

### wolfsrc breakdown

### renderer
```c
jpeg-6/
    jcapimin.c 
    jchuff.c 
    jcinit.c 
    jccoefct.c 
    jccolor.c 
    jfdctflt.c 
    jcdctmgr.c 
    jcphuff.c 
    jcmainct.c 
    jcmarker.c 
    jcmaster.c 
    jcomapi.c
    jcparam.c
    jcprepct.c
    jcsample.c 
    jdapimin.c 
    jdapistd.c 
    jdatasrc.c 
    jdcoefct.c 
    jdcolor.c 
    jddctmgr.c 
    jdhuff.c 
    jdinput.c 
    jdmainct.c 
    jdmarker.c 
    jdmaster.c 
    jdpostct.c 
    jdsample.c 
    jdtrans.c 
    jerror.c  
    jidctflt.c 
    jmemmgr.c 
    jmemnobs.c   
    jutils.c 
renderer/
    tr_animation.c   
    tr_backend.c   
    tr_bsp.c   
    tr_cmds.c   
    tr_cmesh.c
    tr_curve.c   
    tr_flares.c   
    tr_font.c    
    tr_image.c    
    tr_init.c     
    tr_light.c   
    tr_main.c    
    tr_marks.c    
    tr_mesh.c    
    tr_model.c    
    tr_noise.c    
    tr_scene.c    
    tr_shade.c    
    tr_shade_calc.c   
    tr_shader.c    
    tr_shadows.c    
    tr_sky.c    
    tr_surface.c   
    tr_world.c
unix/
    linux_glimp.c
    linux_qgl.c
win32/
    win_gamma.c
    win_glimp.c
    win_qgl.c
```

```c
splines/
    math_angles.cpp
    math_matrix.cpp
    math_quaternion.cpp
    math_vector.cpp
    q_parse.cpp
    q_shared.cpp
    splines.cpp
    util_str.cpp
```

### botlib
```c
botlib/
    be_aas_bspq3.c
    be_aas_cluster.c
    be_aas_debug.c
    be_aas_entity.c
    be_aas_file.c
    be_aas_main.c
    be_aas_move.c
    be_aas_optimize.c
    be_aas_reach.c
    be_aas_route.c
    be_aas_routealt.c
    be_aas_routetable.c
    be_aas_sample.c
    be_ai_char.c
    be_ai_chat.c
    be_ai_gen.c
    be_ai_goal.c
    be_ai_move.c
    be_ai_weap.c
    be_ai_weight.c
    be_ea.c
    be_interface.c
    l_crc.c
    l_libvar.c
    l_log.c
    l_memory.c
    l_precomp.c
    l_script.c
    l_struct.c
```

### common
```c
game/
    q_math.c
    q_shared.c
qcommon/
    cm_load.c
    cm_patch.c
    cm_polylib.c
    cm_test.c
    cm_trace.c
    cmd.c
    common.c
    cvar.c
    files.c
    huffman.c
    md4.c
    msg.c
    net_chan.c
    unzip.c
    vm.c
    vm_interpreted.c
    vm_x86.c
server/
    sv_bot.c
    sv_ccmds.c
    sv_client.c
    sv_game.c
    sv_init.c
    sv_main.c
    sv_net_chan.c
    sv_snapshot.c
    sv_world.c
unix/
    linux_common.c
    unix_main.c
    unix_net.c
    unix_shared.c
    linux_signals.c
win32/
    win_main.c
    win_net.c
    win_shared.c
    win_syscon.c
    win_wndproc.c
```

### client
```c
client/
    cl_cgame.c
    cl_cin.c
    cl_console.c
    cl_input.c
    cl_keys.c
    cl_main.c
    cl_net_chan.c
    cl_parse.c
    cl_scrn.c
    cl_ui.c
    snd_adpcm.c
    snd_dma.c
    snd_mem.c
    snd_mix.c
    snd_wavelet.c

unix/
    linux_qgl.c   
    linux_glimp.c   
    linux_joystick.c  
    linux_snd.c

win32/
    win_input.c
    win_snd.c
```

### wolfded
```c
null/
    null_client.c
    null_input.c
    null_sndma.c
```

## Incomplete Functions

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
