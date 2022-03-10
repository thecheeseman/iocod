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
 * @file system.h
 * @date 2022-02-04
*/

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

//
//
// system/events.c
//
//
void sys_events_init(void);
void sys_queue_event(int time, enum system_event_type type,
					 int value, int value2, int ptrlength, void *ptr);
struct system_event sys_get_event(void);

//
//
// system/shared.c
//
//
void sys_mkdir(const char *path);
void sys_end_streamed_file(filehandle f);
int sys_milliseconds(void);
char *sys_get_current_user(void);
char *sys_cwd(void);
char *sys_default_base_path(void);
char *sys_default_install_path(void);
char *sys_default_home_path(void);
void sys_set_default_cd_path(const char *path);
char *sys_default_cd_path(void);
bool sys_directory_has_files(const char *ospath);
void sys_list_filtered_files(const char *basedir, char *subdirs, char *filter,
							 char **list, int *numfiles);
char **sys_list_files(const char *directory, const char *extension,
					  char *filter, int *numfiles, bool wantsubs);
void sys_free_file_list(char **list);

//
//
// system/system.c
//
//
void sys_do_start_process(char *cmdline);
void sys_exit(int ex);
void sys_quit(void);
void sys_init(void);
void sys_check_for_version(int argc, char *argv[]);
void sys_configure_fpu(void);
void sys_error(const char *error, ...);
void sys_warn(char *warning, ...);
void sys_show_console(int viewlog, int b);
void *sys_load_dll(const char *name, char *fqpath,
				   intptr_t (**entrypoint)(intptr_t, ...), 
				   intptr_t (*systemcalls)(intptr_t, ...));
void sys_unload_dll(void *dllhandle);

//
//
// system/tty.c
//
//
extern bool ttycon_on;
extern struct cvar *tty_colors;
void sys_console_input_shutdown(void);
void sys_console_input_init(void);
char *sys_console_input(void);
void sys_print(const char *msg);
void tty_hide(void);
void tty_show(void);

#endif // __SYSTEM_H__