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
 * @file script_prototypes.c
 * @date 2022-02-04
*/

#include "script_local.h"
#include "common/memory.h"

#include "../game/script/script_redirects.h"

bool scr_get_bool(UNUSED int32_t index)
{
    return false;
}

int32_t scr_get_int(UNUSED int32_t index)
{
    return 0;
}

int32_t scr_get_anim(UNUSED intptr_t a, UNUSED int32_t b, UNUSED int32_t c)
{
    return 0;
}

int32_t scr_get_anim_tree(UNUSED intptr_t a, UNUSED int32_t b)
{
    return 0;
}

float scr_get_float(UNUSED int32_t index)
{
    return 0.0;
}

char *scr_get_string(UNUSED int32_t index)
{
    return NULL;
}

const char *scr_get_const_string(UNUSED int32_t index)
{
    return NULL;
}

const char *scr_get_debug_string(UNUSED int32_t index)
{
    return NULL;
}

char *scr_get_istring(UNUSED int32_t index)
{
    return NULL;
}

const char *scr_get_const_istring(UNUSED int32_t index)
{
    return NULL;
}

void scr_get_vector(UNUSED int32_t index, UNUSED vec3_t v)
{

}

int32_t scr_get_func(UNUSED int32_t index)
{
    return 0;
}

int32_t scr_get_type(UNUSED int32_t index)
{
    return 0;
}

int32_t scr_get_pointer_type(UNUSED int32_t index)
{
    return 0;
}

int32_t scr_get_entity_num(UNUSED int32_t index, UNUSED gentity_t *entity)
{
    return 0;
}

int32_t scr_get_num_param(void)
{
    return 0;
}

void scr_add_bool(UNUSED bool value)
{

}

void scr_add_int(UNUSED int32_t value)
{

}

void scr_add_float(UNUSED float value)
{

}

void scr_add_anim(UNUSED int32_t value)
{

}

void scr_add_undefined(void)
{

}

void scr_add_entity_num(UNUSED int32_t num, UNUSED int32_t b)
{

}

void scr_add_struct(UNUSED int32_t value)
{

}

void scr_add_string(UNUSED char *string)
{

}

void scr_add_istring(UNUSED char *string)
{

}

void scr_add_const_string(UNUSED uint16_t string)
{

}

void scr_add_vector(UNUSED vec3_t vector)
{

}

void scr_add_object(UNUSED uint16_t value)
{

}

void scr_add_array(void)
{

}

void scr_add_array_string_indexed(UNUSED uint16_t string)
{

}

void scr_make_array(void)
{

}

void scr_begin_load_scripts(void)
{

}

void scr_begin_load_anim_trees(void)
{

}

void scr_end_load_scripts(void)
{

}

void scr_end_load_anim_trees(void)
{

}

void scr_precache_anim_trees(void)
{

}

void scr_free_scripts(void)
{

}

void scr_free_game_variable(void)
{

}

void scr_shutdown_system(void)
{

}

bool scr_is_system_active(void)
{
    return false;
}

void scr_add_exec_thread(UNUSED int32_t a, UNUSED int32_t b)
{

}

void 
scr_add_exec_thread_ent_num(UNUSED int32_t a, 
                            UNUSED int32_t b, 
                            UNUSED int32_t c, 
                            UNUSED int32_t d)
{

}

uint16_t scr_exec_thread(UNUSED int32_t a, UNUSED int32_t b)
{
    return 0;
}

uint16_t 
scr_exec_thread_ent_num(UNUSED int32_t a, 
                        UNUSED int32_t b, 
                        UNUSED int32_t c, 
                        UNUSED int32_t d)
{
    return 0;
}

bool SCRIPT_UNUSED scr_is_thread_alive(UNUSED uint16_t thread)
{
    return false;
}

void scr_error(UNUSED char *msg)
{

}

void scr_error_with_dialog_message(UNUSED char *msg)
{

}

void scr_param_error(UNUSED int32_t index, UNUSED char *msg)
{

}

void scr_object_error(UNUSED int32_t a)
{

}

void scr_set_dynamic_entity_field(UNUSED int32_t a, 
                                  UNUSED int32_t b, 
                                  UNUSED int32_t c)
{

}

void scr_free_entity_num(UNUSED int32_t a, UNUSED int32_t b)
{

}

uint16_t scr_get_entity_id(UNUSED int32_t a, UNUSED int32_t b)
{
    return 0;
}

void scr_set_class_map(UNUSED int32_t a, UNUSED int32_t b)
{

}

void scr_remove_class_map(void)
{

}

void SCRIPT_UNUSED scr_spacer1(void)
{

}

void SCRIPT_UNUSED scr_spacer2(void)
{

}

void scr_add_class_field(UNUSED uint16_t a, UNUSED int32_t b, UNUSED uint16_t c)
{

}

void scr_add_fields(UNUSED int32_t a, UNUSED int32_t b)
{

}

uint16_t scr_find_field(UNUSED int32_t a, UNUSED int32_t b)
{
    return 0;
}

int32_t scr_get_offset(UNUSED uint16_t a, UNUSED int32_t b)
{
    return 0;
}

void scr_copy_entity_num(UNUSED int32_t a, UNUSED int32_t b, UNUSED int32_t c)
{

}

void scr_init(UNUSED int32_t a, UNUSED int32_t b)
{

}

void scr_shutdown(void)
{

}

void scr_abort(void)
{

}

void scr_set_loading(UNUSED int32_t a)
{

}

void scr_init_system(UNUSED int32_t a, UNUSED int32_t b)
{

}

void scr_alloc_game_variable(void)
{

}

void SCRIPT_UNUSED scr_get_checksum(UNUSED int32_t a)
{

}

void SCRIPT_UNUSED scr_has_source_files(void)
{

}

void SCRIPT_UNUSED scr_save_source(UNUSED int32_t a)
{

}

void SCRIPT_UNUSED scr_load_source(UNUSED int32_t a)
{

}

void SCRIPT_UNUSED scr_skip_source(UNUSED int32_t a)
{

}

void SCRIPT_UNUSED scr_save_pre(UNUSED int32_t a)
{

}

void SCRIPT_UNUSED scr_save_post(UNUSED int32_t a)
{

}

void SCRIPT_UNUSED scr_save_shutdown(void)
{

}

void SCRIPT_UNUSED scr_spacer3(void)
{

}

void SCRIPT_UNUSED scr_load_pre(UNUSED int32_t a, UNUSED int32_t b)
{

}

void SCRIPT_UNUSED scr_load_shutdown(void)
{

}

void SCRIPT_UNUSED scr_spacer4(void)
{

}

int32_t scr_load_script(UNUSED char *script)
{
    return 0;
}

intptr_t scr_find_anim_tree(UNUSED intptr_t a, UNUSED int32_t b)
{
    return 0;
}

void scr_find_anim(UNUSED char *animtree, UNUSED char *name, UNUSED intptr_t anim)
{

}

int32_t scr_get_function_handle(UNUSED char *script, UNUSED char *function)
{
    return 0;
}

void scr_free_thread(UNUSED uint16_t thread)
{

}

uint16_t SCRIPT_UNUSED scr_convert_thread_to_save(UNUSED uint16_t thread)
{
    return 0;
}

uint16_t SCRIPT_UNUSED scr_convert_thread_from_load(UNUSED uint16_t thread)
{
    return 0;
}

void scr_set_string(UNUSED int32_t a, UNUSED uint16_t string)
{

}

uint16_t scr_alloc_string(UNUSED char *string, UNUSED int32_t b)
{
    return 0;
}

void scr_notify_num(UNUSED int32_t a, UNUSED int32_t b, UNUSED uint16_t c, UNUSED int32_t d)
{

}

void scr_notify_id(UNUSED uint16_t a, UNUSED uint16_t b, UNUSED int32_t c)
{

}

char *sl_convert_to_string(UNUSED uint16_t string)
{
    return NULL;
}

uint16_t sl_get_string(UNUSED int32_t a, UNUSED byte b)
{
    return 0;
}

uint16_t sl_get_lowercase_string(UNUSED int32_t a, UNUSED byte b)
{
    return 0;
}

uint16_t sl_find_lowercase_string(UNUSED int32_t a)
{
    return 0;
}

uint16_t SCRIPT_UNUSED scr_create_canonical_filename(UNUSED int32_t a)
{
    return 0;
}

void scr_set_time(UNUSED int32_t time)
{

}

void scr_run_current_threads(void)
{

}

void scr_reset_timeout(void)
{

}

int32_t scr_get_anims_index(UNUSED int32_t a)
{
    return 0;
}

void SCRIPT_UNUSED scr_get_anims(UNUSED int32_t a)
{

}

void *mt_alloc(UNUSED int32_t a, UNUSED int32_t b)
{
    return NULL;
}

void mt_free(UNUSED void *a, UNUSED int32_t b)
{

}

//
//
struct gsc_function *(*_scr_get_function)(const char **name, int *developer);
struct gsc_function *(*_scr_get_method)(const char **name, int *developer);
void (*_scr_set_object_field)(int a, int b, int c);
void (*_scr_get_object_field)(int a, int b, int c);
int (*_scr_load_read)(int a);
//
//

struct gsc_function *scr_get_function(const char **name, int *developer)
{
    return (*_scr_get_function)(name, developer);
}

struct gsc_function *scr_get_method(const char **name, int *developer)
{
    return (*_scr_get_method)(name, developer);
}

void scr_set_object_field(int a, int b, int c)
{
    (*_scr_set_object_field)(a, b, c);
}

void scr_get_object_field(int a, int b, int c)
{
    (*_scr_get_object_field)(a, b, c);
}

int scr_load_read(int a)
{
    return (*_scr_load_read)(a);
}

intptr_t scr_far_hook(intptr_t addr)
{
    // game dll will pass us the address of its internal scr_get_function()
    // so just copy that address directly into our local prototypes
    if (addr)
        com_memcpy(&_scr_get_function, &addr, sizeof(intptr_t) * 5);

    _scr_get_bool = scr_get_bool;
    _scr_get_int = scr_get_int;
    _scr_get_anim = scr_get_anim;
    _scr_get_anim_tree = scr_get_anim_tree;
    _scr_get_float = scr_get_float;
    _scr_get_string = scr_get_string;
    _scr_get_const_string = scr_get_const_string;
    _scr_get_debug_string = scr_get_debug_string;
    _scr_get_istring = scr_get_istring;
    _scr_get_const_istring = scr_get_const_istring;
    _scr_get_vector = scr_get_vector;
    _scr_get_func = scr_get_func;
    _scr_get_type = scr_get_type;
    _scr_get_pointer_type = scr_get_pointer_type;
    _scr_get_entity_num = scr_get_entity_num;
    _scr_get_num_param = scr_get_num_param;
    _scr_add_bool = scr_add_bool;
    _scr_add_int = scr_add_int;
    _scr_add_float = scr_add_float;
    _scr_add_anim = scr_add_anim;
    _scr_add_undefined = scr_add_undefined;
    _scr_add_entity_num = scr_add_entity_num;
    _scr_add_struct = scr_add_struct;
    _scr_add_string = scr_add_string;
    _scr_add_istring = scr_add_istring;
    _scr_add_const_string = scr_add_const_string;
    _scr_add_vector = scr_add_vector;
    _scr_add_object = scr_add_object;
    _scr_add_array = scr_add_array;
    _scr_add_array_string_indexed = scr_add_array_string_indexed;
    _scr_make_array = scr_make_array;
    _scr_begin_load_scripts = scr_begin_load_scripts;
    _scr_begin_load_anim_trees = scr_begin_load_anim_trees;
    _scr_end_load_scripts = scr_end_load_scripts;
    _scr_end_load_anim_trees = scr_end_load_anim_trees;
    _scr_precache_anim_trees = scr_precache_anim_trees;
    _scr_free_scripts = scr_free_scripts;
    _scr_free_game_variable = scr_free_game_variable;
    _scr_shutdown_system = scr_shutdown_system;
    _scr_is_system_active = scr_is_system_active;
    _scr_add_exec_thread = scr_add_exec_thread;
    _scr_add_exec_thread_ent_num = scr_add_exec_thread_ent_num;
    _scr_exec_thread = scr_exec_thread;
    _scr_exec_thread_ent_num = scr_exec_thread_ent_num;
    _scr_is_thread_alive = scr_is_thread_alive;
    _scr_error = scr_error;
    _scr_error_with_dialog_message = scr_error_with_dialog_message;
    _scr_param_error = scr_param_error;
    _scr_object_error = scr_object_error;
    _scr_set_dynamic_entity_field = scr_set_dynamic_entity_field;
    _scr_free_entity_num = scr_free_entity_num;
    _scr_get_entity_id = scr_get_entity_id;
    _scr_set_class_map = scr_set_class_map;
    _scr_remove_class_map = scr_remove_class_map;
    _scr_add_class_field = scr_add_class_field;
    _scr_add_fields = scr_add_fields;
    _scr_find_field = scr_find_field;
    _scr_get_offset = scr_get_offset;
    _scr_copy_entity_num = scr_copy_entity_num;
    _scr_init = scr_init;
    _scr_shutdown = scr_shutdown;
    _scr_abort = scr_abort;
    _scr_set_loading = scr_set_loading;
    _scr_init_system = scr_init_system;
    _scr_alloc_game_variable = scr_alloc_game_variable;
    _scr_get_checksum = scr_get_checksum;
    _scr_has_source_files = scr_has_source_files;
    _scr_save_source = scr_save_source;
    _scr_load_source = scr_load_source;
    _scr_skip_source = scr_skip_source;
    _scr_save_pre = scr_save_pre;
    _scr_save_post = scr_save_post;
    _scr_save_shutdown = scr_save_shutdown;
    _scr_load_pre = scr_load_pre;
    _scr_load_shutdown = scr_load_shutdown;
    _scr_load_script = scr_load_script;
    _scr_find_anim_tree = scr_find_anim_tree;
    _scr_find_anim = scr_find_anim;
    _scr_get_function_handle = scr_get_function_handle;
    _scr_free_thread = scr_free_thread;
    _scr_convert_thread_to_save = scr_convert_thread_to_save;
    _scr_convert_thread_from_load = scr_convert_thread_from_load;
    _scr_set_string = scr_set_string;
    _scr_alloc_string = scr_alloc_string;
    _scr_notify_num = scr_notify_num;
    _scr_notify_id = scr_notify_id;
    _sl_convert_to_string = sl_convert_to_string;
    _sl_get_string = sl_get_string;
    _sl_get_lowercase_string = sl_get_lowercase_string;
    _sl_find_lowercase_string = sl_find_lowercase_string;
    _scr_create_canonical_filename = scr_create_canonical_filename;
    _scr_set_time = scr_set_time;
    _scr_run_current_threads = scr_run_current_threads;
    _scr_reset_timeout = scr_reset_timeout;
    _scr_get_anims_index = scr_get_anims_index;
    _scr_get_anims = scr_get_anims;
    _mt_alloc = mt_alloc;
    _mt_free = mt_free;

    // pass back to the dll the address of our functions that it needs
    // access to
    return (intptr_t) &_scr_get_bool;
}
