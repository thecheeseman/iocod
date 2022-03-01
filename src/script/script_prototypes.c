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

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

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
		memcpy(&_scr_get_function, &addr, sizeof(intptr_t) * 5);

	_scr_get_bool = scr_get_bool;
	_scr_get_int = scr_get_int;
	_scr_get_anim = scr_get_anim;
	_scr_get_animtree = scr_get_animtree;
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
	_scr_begin_load_animtrees = scr_begin_load_animtrees;
	_scr_end_load_scripts = scr_end_load_scripts;
	_scr_end_load_animtrees = scr_end_load_animtrees;
	_scr_precache_animtrees = scr_precache_animtrees;
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
	_scr_find_animtree = scr_find_animtree;
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

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
