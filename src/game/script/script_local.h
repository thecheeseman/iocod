#ifndef __GAME_SCRIPT_LOCAL_H__
#define __GAME_SCRIPT_LOCAL_H__

#include "../game_local.h"
#include "global/script.h"

void scr_parse_gametype_list(void);
void scr_load_gametype(void);
void scr_load_level(void);
void scr_startup_gametype(void);

//
// gsc functions
//
extern struct gsc_function gsc_functions[];
extern struct gsc_method gsc_methods[];

#endif // __GAME_SCRIPT_LOCAL_H__
