#include "script_local.h"
#include "script_redirects.h"

bool scr_get_bool(int32_t index)
{
    return (*_scr_get_bool)(index);
}

int32_t scr_get_int(int32_t index)
{
    return (*_scr_get_int)(index);
}

int32_t scr_get_anim(intptr_t a, int32_t b, int32_t c)
{
    return (*_scr_get_anim)(a, b, c);
}

int32_t scr_get_anim_tree(intptr_t a, int32_t b)
{
    return (*_scr_get_anim_tree)(a, b);
}

float scr_get_float(int32_t index)
{
    return (*_scr_get_float)(index);
}

char *scr_get_string(int32_t index)
{
    return (*_scr_get_string)(index);
}

const char *scr_get_const_string(int32_t index)
{
    return (*_scr_get_const_string)(index);
}

const char *scr_get_debug_string(int32_t index)
{
    return (*_scr_get_debug_string)(index);
}

char *scr_get_istring(int32_t index)
{
    return (*_scr_get_istring)(index);
}

const char *scr_get_const_istring(int32_t index)
{
    return (*_scr_get_const_istring)(index);
}

void scr_get_vector(int32_t index, vec3_t v)
{
    (*_scr_get_vector)(index, v);
}

int32_t scr_get_func(int32_t index)
{
    return (*_scr_get_func)(index);
}

int32_t scr_get_type(int32_t index)
{
    return (*_scr_get_type)(index);
}

int32_t scr_get_pointer_type(int32_t index)
{
    return (*_scr_get_pointer_type)(index);
}

int32_t scr_get_entity_num(int32_t index, gentity_t *entity)
{
    return (*_scr_get_entity_num)(index, entity);
}

int32_t scr_get_num_param(void)
{
    return (*_scr_get_num_param)();
}

void scr_add_bool(bool value)
{
    (*_scr_add_bool)(value);
}

void scr_add_int(int32_t value)
{
    (*_scr_add_int)(value);
}

void scr_add_float(float value)
{
    (*_scr_add_float)(value);
}

void scr_add_anim(int32_t value)
{
    (*_scr_add_anim)(value);
}

void scr_add_undefined(void)
{
    (*_scr_add_undefined)();
}

void scr_add_entity_num(int32_t num, int32_t b)
{
    (*_scr_add_entity_num)(num, b);
}

void scr_add_struct(int32_t value)
{
    (*_scr_add_struct)(value);
}

void scr_add_string(char *string)
{
    (*_scr_add_string)(string);
}

void scr_add_istring(char *string)
{
    (*_scr_add_istring)(string);
}

void scr_add_const_string(uint16_t string)
{
    (*_scr_add_const_string)(string);
}

void scr_add_vector(vec3_t vector)
{
    (*_scr_add_vector)(vector);
}

void scr_add_object(uint16_t value)
{
    (*_scr_add_object)(value);
}

void scr_add_array(void)
{
    (*_scr_add_array)();
}

void scr_add_array_string_indexed(uint16_t string)
{
    (*_scr_add_array_string_indexed)(string);
}

void scr_make_array(void)
{
    (*_scr_make_array)();
}

void scr_begin_load_scripts(void)
{
    (*_scr_begin_load_scripts)();
}

void scr_begin_load_anim_trees(void)
{
    (*_scr_begin_load_anim_trees)();
}

void scr_end_load_scripts(void)
{
    (*_scr_end_load_scripts)();
}

void scr_end_load_anim_trees(void)
{
    (*_scr_end_load_anim_trees)();
}

void scr_precache_anim_trees(void)
{
    (*_scr_precache_anim_trees)();
}

void scr_free_scripts(void)
{
    (*_scr_free_scripts)();
}

void scr_free_game_variable(void)
{
    (*_scr_free_game_variable)();
}

void scr_shutdown_system(void)
{
    (*_scr_shutdown_system)();
}

bool scr_is_system_active(void)
{
    return (*_scr_is_system_active)();
}

void scr_add_exec_thread(int32_t a, int32_t b)
{
    (*_scr_add_exec_thread)(a, b);
}

void scr_add_exec_thread_ent_num(int32_t a, int32_t b, int32_t c, int32_t d)
{
    (*_scr_add_exec_thread_ent_num)(a, b, c, d);
}

uint16_t scr_exec_thread(int32_t a, int32_t b)
{
    return (*_scr_exec_thread)(a, b);
}

uint16_t scr_exec_thread_ent_num(int32_t a, int32_t b, int32_t c, int32_t d)
{
    return (*_scr_exec_thread_ent_num)(a, b, c, d);
}

bool SCRIPT_UNUSED scr_is_thread_alive(uint16_t thread)
{
    return (*_scr_is_thread_alive)(thread);
}

void scr_error(char *msg)
{
    (*_scr_error)(msg);
}

void scr_error_with_dialog_message(char *msg)
{
    (*_scr_error_with_dialog_message)(msg);
}

void scr_param_error(int32_t index, char *msg)
{
    (*_scr_param_error)(index, msg);
}

void scr_object_error(int32_t a)
{
    (*_scr_object_error)(a);
}

void scr_set_dynamic_entity_field(int32_t a, int32_t b, int32_t c)
{
    (*_scr_set_dynamic_entity_field)(a, b, c);
}

void scr_free_entity_num(int32_t a, int32_t b)
{
    (*_scr_free_entity_num)(a, b);
}

uint16_t scr_get_entity_id(int32_t a, int32_t b)
{
    return (*_scr_get_entity_id)(a, b);
}

void scr_set_class_map(int32_t a, int32_t b)
{
    (*_scr_set_class_map)(a, b);
}

void scr_remove_class_map(void)
{
    (*_scr_remove_class_map)();
}

void SCRIPT_UNUSED scr_spacer1(void)
{
    (*_scr_spacer1)();
}

void SCRIPT_UNUSED scr_spacer2(void)
{
    (*_scr_spacer2)();
}

void scr_add_class_field(uint16_t a, int32_t b, uint16_t c)
{
    (*_scr_add_class_field)(a, b, c);
}

void scr_add_fields(int32_t a, int32_t b)
{
    (*_scr_add_fields)(a, b);
}

uint16_t scr_find_field(int32_t a, int32_t b)
{
    return (*_scr_find_field)(a, b);
}

int32_t scr_get_offset(uint16_t a, int32_t b)
{
    return (*_scr_get_offset)(a, b);
}

void scr_copy_entity_num(int32_t a, int32_t b, int32_t c)
{
    (*_scr_copy_entity_num)(a, b, c);
}

void scr_init(int32_t a, int32_t b)
{
    (*_scr_init)(a, b);
}

void scr_shutdown(void)
{
    (*_scr_shutdown)();
}

void scr_abort(void)
{
    (*_scr_abort)();
}

void scr_set_loading(int32_t a)
{
    (*_scr_set_loading)(a);
}

void scr_init_system(int32_t a, int32_t b)
{
    (*_scr_init_system)(a, b);
}

void scr_alloc_game_variable(void)
{
    (*_scr_alloc_game_variable)();
}

void SCRIPT_UNUSED scr_get_checksum(int32_t a)
{
    (*_scr_get_checksum)(a);
}

void SCRIPT_UNUSED scr_has_source_files(void)
{
    (*_scr_has_source_files)();
}

void SCRIPT_UNUSED scr_save_source(int32_t a)
{
    (*_scr_save_source)(a);
}

void SCRIPT_UNUSED scr_load_source(int32_t a)
{
    (*_scr_load_source)(a);
}

void SCRIPT_UNUSED scr_skip_source(int32_t a)
{
    (*_scr_skip_source)(a);
}

void SCRIPT_UNUSED scr_save_pre(int32_t a)
{
    (*_scr_save_pre)(a);
}

void SCRIPT_UNUSED scr_save_post(int32_t a)
{
    (*_scr_save_post)(a);
}

void SCRIPT_UNUSED scr_save_shutdown(void)
{
    (*_scr_save_shutdown)();
}

void SCRIPT_UNUSED scr_spacer3(void)
{
    (*_scr_spacer3)();
}

void SCRIPT_UNUSED scr_load_pre(int32_t a, int32_t b)
{
    (*_scr_load_pre)(a, b);
}

void SCRIPT_UNUSED scr_load_shutdown(void)
{
    (*_scr_load_shutdown)();
}

void SCRIPT_UNUSED scr_spacer4(void)
{
    (*_scr_spacer4)();
}

int32_t scr_load_script(char *script)
{
    return (*_scr_load_script)(script);
}

intptr_t scr_find_anim_tree(intptr_t a, int32_t b)
{
    return (*_scr_find_anim_tree)(a, b);
}

void scr_find_anim(char *animtree, char *name, intptr_t anim)
{
    (*_scr_find_anim)(animtree, name, anim);
}

int32_t scr_get_function_handle(char *script, char *function)
{
    return (*_scr_get_function_handle)(script, function);
}

void scr_free_thread(uint16_t thread)
{
    (*_scr_free_thread)(thread);
}

uint16_t SCRIPT_UNUSED scr_convert_thread_to_save(uint16_t thread)
{
    return (*_scr_convert_thread_to_save)(thread);
}

uint16_t SCRIPT_UNUSED scr_convert_thread_from_load(uint16_t thread)
{
    return (*_scr_convert_thread_from_load)(thread);
}

void scr_set_string(int32_t a, uint16_t string)
{
    (*_scr_set_string)(a, string);
}

uint16_t scr_alloc_string(char *string, int32_t b)
{
    return (*_scr_alloc_string)(string, b);
}

void scr_notify_num(int32_t a, int32_t b, uint16_t c, int32_t d)
{
    (*_scr_notify_num)(a, b, c, d);
}

void scr_notify_id(uint16_t a, uint16_t b, int32_t c)
{
    (*_scr_notify_id)(a, b, c);
}

char *sl_convert_to_string(uint16_t string)
{
    return (*_sl_convert_to_string)(string);
}

uint16_t sl_get_string(int32_t a, byte b)
{
    return (*_sl_get_string)(a, b);
}

uint16_t sl_get_lowercase_string(int32_t a, byte b)
{
    return (*_sl_get_lowercase_string)(a, b);
}

uint16_t sl_find_lowercase_string(int32_t a)
{
    return (*_sl_find_lowercase_string)(a);
}

uint16_t SCRIPT_UNUSED scr_create_canonical_filename(int32_t a)
{
    return (*_scr_create_canonical_filename)(a);
}

void scr_set_time(int32_t time)
{
    (*_scr_set_time)(time);
}

void scr_run_current_threads(void)
{
    (*_scr_run_current_threads)();
}

void scr_reset_timeout(void)
{
    (*_scr_reset_timeout)();
}

int32_t scr_get_anims_index(int32_t a)
{
    return (*_scr_get_anims_index)(a);
}

void SCRIPT_UNUSED scr_get_anims(int32_t a)
{
    (*_scr_get_anims)(a);
}

void *mt_alloc(int32_t a, int32_t b)
{
    return (*_mt_alloc)(a, b);
}

void mt_free(void *a, int32_t b)
{
    (*_mt_free)(a, b);
}

