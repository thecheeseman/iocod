#ifndef __GAME_SCRIPT_LOCAL_H__
#define __GAME_SCRIPT_LOCAL_H__

#include "../game_local.h"
#include "global/script.h"

//
//
// script_main.c
//
//
void scr_parse_gametype_list(void);
void scr_load_gametype(void);
void scr_load_level(void);
void scr_startup_gametype(void);

//
//
// script_misc.c
//
//
char *get_gametype_name_for_script(const char *str);
bool is_valid_gametype(const char *gametype);

//
// gsc functions
//
extern struct gsc_function gsc_functions[];
extern struct gsc_method gsc_methods[];

#endif // __GAME_SCRIPT_LOCAL_H__
