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
 * @file game_syscalls.c
 * @date 2022-02-04
*/

#include "game_local.h"

static intptr_t (QDECL *syscall)(intptr_t arg, ...) = 
	(intptr_t (QDECL *)(intptr_t, ...)) -1;

void DLLEXPORT dllEntry(intptr_t(QDECL *syscallptr)(intptr_t arg, ...))
{
	syscall = syscallptr;
}

/* 0 */
void trap_printf(const char *fmt) 
{
	syscall(G_PRINT, fmt);
}

/* 1 */
void trap_error(const char *fmt) 
{
	syscall(G_ERROR, fmt);
}

/* 2 */
void trap_error_localized(const char *fmt)
{
	syscall(G_ERROR_LOCALIZED, fmt);
}

/* 3 */
int trap_milliseconds(void) 
{
	return syscall(G_MILLISECONDS);
}

/* 4 */
void trap_cvar_register(struct vmcvar *cvar, const char *var_name,
						const char *value, int flags) {
	syscall(G_CVAR_REGISTER, cvar, var_name, value, flags);
}

/* 5 */
void trap_cvar_update(struct vmcvar *cvar)
{
	syscall(G_CVAR_UPDATE, cvar);
}

/* 6 */
void trap_cvar_set(const char *var_name, const char *value)
{
	syscall(G_CVAR_SET, var_name, value);
}

/* 7 */
int trap_cvar_variable_integer_value(const char *var_name)
{
	return syscall(G_CVAR_VARIABLE_INTEGER_VALUE, var_name);
}

/* 8 */
float trap_cvar_variable_value(const char *var_name)
{
	float flt;

	syscall(G_CVAR_VARIABLE_VALUE, var_name, &flt);
	return flt;
}

/* 9 */
void trap_cvar_variable_string_buffer(const char *var_name, char *buffer,
									  int size)
{
	syscall(G_CVAR_VARIABLE_STRING_BUFFER, var_name, buffer, size);
}

/* 10 */
int trap_argc(void)
{ 
	return syscall(G_ARGC);
}

/* 11 */
void trap_argv(int n, char *buffer, int buffer_length)
{
	syscall(G_ARGV, n, buffer, buffer_length);
}

/* 12 */
void *trap_hunk_alloc_internal(int size)
{
	return (void *) syscall(G_HUNK_ALLOC_LOW_INTERNAL, size);
}

/* 13 */
void *trap_hunk_alloc_low_internal(int size)
{
	return (void *) syscall(G_HUNK_ALLOC_LOW_INTERNAL, size);
}

/* 14 */
void *trap_hunk_alloc_align_internal(int size, int align)
{
	return (void *) syscall(G_HUNK_ALLOC_ALIGN_INTERNAL, size, align);
}

/* 15 */
void *trap_hunk_alloc_low_align_internal(int size, int align)
{
	return (void *) syscall(G_HUNK_ALLOC_LOW_ALIGN_INTERNAL, size, align);
}

/* 16 */
void *trap_hunk_allocate_temp_memory_internal(int size)
{
	return (void *) syscall(G_HUNK_ALLOCATE_TEMP_MEMORY_INTERNAL, size);
}

/* 17 */
void trap_hunk_free_temp_memory_internal(int size)
{
	syscall(G_HUNK_FREE_TEMP_MEMORY_INTERNAL, size);
}

/* 18 */
int trap_fs_fopen_file(const char *qpath, filehandle *f, enum fs_mode mode)
{
	return syscall(G_FS_FOPEN_FILE, qpath, f, mode);
}

/* 19 */
void trap_fs_read(void *buffer, int len, filehandle f)
{
	syscall(G_FS_READ, buffer, len, f);
}

/* 20 */
int trap_fs_write(const void *buffer, int len, filehandle f)
{
	return syscall(G_FS_WRITE, buffer, len, f);
}

/* 21 */
int trap_fs_rename(const char *from, const char *to)
{
	return syscall(G_FS_RENAME, from, to);
}

/* 22 */
void trap_fs_fclose_file(filehandle file)
{
	syscall(G_FS_FCLOSE_FILE, file);
}

/* 23 */
void trap_send_console_command(int exec_when, const char *text)
{
	syscall(G_SEND_CONSOLE_COMMAND, exec_when, text);
}

/* 24 */
void trap_locate_game_data(struct gentity *gents, int num_gentities,
						   int gentity_size, struct player_state *clients,
						   int client_size)
{
	syscall(G_LOCATE_GAME_DATA, gents, num_gentities, gentity_size,
			clients, client_size);
}

/* 25 */
void trap_drop_client(int client_num, const char *reason)
{
	syscall(G_DROP_CLIENT, client_num, reason);
}

/* 26 */
void trap_send_server_command(int client, int a, const char *text)
{
	syscall(G_SEND_SERVER_COMMAND, client, a, text);
}

/* 27 */
void trap_set_config_string(int num, const char *string)
{
	syscall(G_SET_CONFIG_STRING, num, string);
}

/* 28 */
void trap_get_config_string(int num, char *buffer, int size)
{
	syscall(G_GET_CONFIG_STRING, num, buffer, size);
}

/* 29 */
const char *trap_get_config_string_const(int num)
{
	return (char *) syscall(G_GET_CONFIG_STRING_CONST, num);
}

/* 32 */
void trap_get_server_info(char *buffer, int size)
{
	syscall(G_GET_SERVERINFO, buffer, size);
}

/* 56 */
int trap_fs_get_file_list(const char *path, const char *extension,
						   char *buffer, int size)
{
	return syscall(G_FS_GET_FILE_LIST, path, extension, buffer, size);
}

/* 132 */
struct weapon **trap_get_weapon_info_memory(int size, int *already_allocated)
{
	return (struct weapon **) syscall(G_GET_WEAPON_INFO_MEMORY, size, 
									  already_allocated);
}

/* 133 */
void trap_free_weapon_info_memory(int a)
{
	syscall(G_FREE_WEAPON_INFO_MEMORY, a);
}
