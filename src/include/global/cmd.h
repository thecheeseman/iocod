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
 * @file cmd.h
 * @date 2022-02-04
*/

#ifndef __CMD_H__
#define __CMD_H__

typedef void (*xcommand)(void);
struct cmd_function {
	struct cmd_function *next;
	char *name;
	xcommand function;

	// added
	int alias_count;
	char **aliases;
};

size_t cmd_argc(void);
char *cmd_argv(size_t arg);
char *cmd_args(void);
char *cmd_args_from(size_t arg);
void cmd_argv_buffer(size_t arg, char *buffer, size_t bufsize);
void cmd_tokenize_string(const char *text_in);
void cmd_add_command(const char *cmd_name, xcommand function);
void cmd_remove_command(const char *cmd_name);
void cmd_add_alias(const char *cmd_name, const char *alias);
void cmd_command_completion(void (*callback)(const char *s));
void cmd_execute_string(const char *text);

//
//
// commands/commands.c
//
//
void cmd_init(void);
void cmd_shutdown(void);

//
//
// commands/common.c
//
//
void com_add_commands(void);
void com_remove_commands(void);

#endif // __CMD_H__
