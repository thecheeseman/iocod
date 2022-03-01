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
 * @file cvar.h
 * @date 2022-02-04
*/

#ifndef __CVAR_H__
#define __CVAR_H__

#define	CVAR_ARCHIVE		1	// set to cause it to be saved to vars.rc
// used for system variables, not for player
// specific configurations
#define	CVAR_USERINFO		2	// sent to server on connect or change
#define	CVAR_SERVERINFO		4	// sent in response to front end requests
#define	CVAR_SYSTEMINFO		8	// these cvars will be duplicated on all clients
#define	CVAR_INIT			16	// don't allow change from console at all,
								// but can be set from the command line
#define	CVAR_LATCH			32	// will only change when C code next does
								// a Cvar_Get(), so it can't be changed
								// without proper initialization.  modified
								// will be set, even though the value hasn't
								// changed yet
#define	CVAR_ROM			64	// display only, cannot be set by user at all
#define	CVAR_USER_CREATED	128	// created by a set command
#define	CVAR_TEMP			256	// can be set even when cheats are disabled, but is not archived
#define CVAR_CHEAT			512	// can not be changed if cheats are disabled
#define CVAR_NORESTART		1024	// do not clear when a cvar_restart is issued

#define CVAR_WOLFINFO		2048

#define CVAR_8192			8192
#define CVAR_8193			(CVAR_8192 | CVAR_ARCHIVE)
#define CVAR_2004			(CVAR_SERVERINFO | CVAR_8192)

// nothing outside the cvar_*() functions should modify these fields!
struct cvar {
	char *name;
	char *string;
	char *reset_string;		// cvar_restart will reset to this value
	char *latched_string;		// for CVAR_LATCH vars
	int	flags;
	bool modified;			// set each time the cvar is changed
	int	modification_count;	// incremented each time the cvar is changed
	float value;				// atof( string )
	int	integer;			// atoi( string )
	struct cvar *next;
	struct cvar *hash_next;
};

#define	MAX_CVAR_VALUE_STRING	256

typedef int	cvarhandle;

// the modules that run in the virtual machine can't access the cvar_t directly,
// so they must ask for structured updates
struct vmcvar {
	cvarhandle handle;
	int	modification_count;
	float value;
	int	integer;
	char string[MAX_CVAR_VALUE_STRING];
};

//
//
// cvar.c
//
//
extern int cvar_modified_flags;

float cvar_variable_value(const char *var_name);
int cvar_variable_integer_value(const char *var_name);
char *cvar_variable_string(const char *var_name);
void cvar_variable_string_buffer(const char *var_name, char *buffer,
								 int bufsize);
void cvar_command_completion(void (*callback)(const char *s));
char *cvar_clean_foreign_characters(const char *value);
struct cvar *cvar_get(const char *var_name, const char *var_value, int flags);
struct cvar *cvar_set2(const char *var_name, const char *value, bool force);
void cvar_set(const char *var_name, const char *value);
void cvar_set_latched(const char *var_name, const char *value);
void cvar_set_value(const char *var_name, float value);
void cvar_reset(const char *var_name);
void cvar_shutdown(void);
void cvar_write_variables(filehandle f);
void cvar_write_defaults(filehandle f);
char *cvar_info_string(int bit);
char *cvar_info_string_big(int bit);
void cvar_update_flags(void);
void cvar_init(void);

//
//
// cvar/cvar_cmds.c
//
//
bool cvar_command(void);
void cvar_add_commands(void);
void cvar_remove_commands(void);

#endif // __CVAR_H__
