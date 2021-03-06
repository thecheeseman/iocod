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

#ifndef CVAR_CVAR_H
#define CVAR_CVAR_H

#include "iocod.h"

#include "types/bool.h"
#include "types/filehandle.h"

#ifdef CVAR_STANDALONE
#include "no_com.h"
#endif

#define MAX_CVARS             1024
#define FILE_HASH_SIZE        256
#define	MAX_CVAR_VALUE_STRING 256

#define	CVAR_ARCHIVE		1	// set to cause it to be saved to vars.rc
// used for system variables, not for player
// specific configurations
#define	CVAR_USER_INFO		2	// sent to server on connect or change
#define	CVAR_SERVER_INFO	4	// sent in response to front end requests
#define	CVAR_SYSTEM_INFO	8	// these cvars will be duplicated on all clients
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
#define CVAR_NO_RESTART		1024	// do not clear when a cvar_restart is issued

#define CVAR_WOLF_INFO		2048

#define CVAR_8192			8192
#define CVAR_8193			(CVAR_8192 | CVAR_ARCHIVE)
#define CVAR_2004			(CVAR_SERVER_INFO | CVAR_8192)

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

extern int cvar_modified_flags;

//
// cvar_string.c
//
/**
 * @brief Validate a cvar string
 * @param s String to validate
 * @return False if string contains '\' or '"' or ';' chars, true otherwise
*/
EXPORT bool cvar_validate_string(const char *s);

/**
 * @brief Return a cvar's string value
 * @param var_name Name of the cvar to search for
 * @return A string containing the value
*/
EXPORT char *cvar_variable_string(const char *var_name);

/**
 * @brief Return a cvar's string value into the given string buffer
 * @param var_name Name of the cvar to search for
 * @param buffer Pointer to string buffer
 * @param bufsize Size of the string buffer
*/
EXPORT void cvar_variable_string_buffer(const char *var_name, char *buffer,
                                 int bufsize);

/**
 * @brief Some cvar values need to be safe from foreign characters
 * @author idsoftware
 * @param value Value to clean
 * @return A cleaned string
*/
EXPORT char *cvar_clean_foreign_characters(const char *value);

//
// cvar_value.c
//
/**
 * @brief Return a cvar's floating point value
 * @param var_name Name of the cvar to search for
 * @return A float containing the value
*/
EXPORT float cvar_variable_value(const char *var_name);

/**
 * @brief Return a cvar's integer value
 * @param var_name Name of the cvar to search for
 * @return An integer containing the value
*/
EXPORT int cvar_variable_integer_value(const char *var_name);

//
//
//
#ifndef CVAR_STANDALONE
void cvar_remove_commands(void);
void cvar_add_commands(void);
void cvar_command_completion(void (*callback)(const char *s));
#endif

/**
 * @brief Return a cvar, or create a new one if it doesn't exist
*/
EXPORT struct cvar *cvar_get(const char *var_name, const char *var_value, int flags);

/**
 * @brief Set a cvar value
*/
EXPORT struct cvar *cvar_set2(const char *var_name, const char *value, bool force);

EXPORT void cvar_set(const char *var_name, const char *value);
EXPORT void cvar_set_latched(const char *var_name, const char *value);
EXPORT void cvar_set_value(const char *var_name, float value);
EXPORT void cvar_reset(const char *var_name);
EXPORT void cvar_shutdown(void);
EXPORT void cvar_write_variables(filehandle f);
EXPORT void cvar_write_defaults(filehandle f);
EXPORT char *cvar_info_string(int bit);
EXPORT char *cvar_info_string_big(int bit);
EXPORT void cvar_update_flags(void);
EXPORT bool cvar_command(void);
EXPORT void cvar_init(void);

#endif /* CVAR_CVAR_H */
