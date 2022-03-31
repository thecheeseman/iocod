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
 * @file common.h
 * @date 2022-02-04
*/

#ifndef __COMMON_GLOBAL_H__
#define __COMMON_GLOBAL_H__

#include "types/bool.h"
#include "system/events.h"

//
//
// common/common.c
//
//
extern struct cvar *com_developer;
extern struct cvar *com_maxfps;
extern struct cvar *com_dedicated;
extern struct cvar *com_timescale;
extern struct cvar *com_version;
extern struct cvar *com_shortversion;
extern struct cvar *com_developer_script;
extern struct cvar *com_logfile;
extern struct cvar *com_fixedtime;
extern struct cvar *com_viewlog;
extern struct cvar *com_speeds;
extern struct cvar *sv_paused;
extern struct cvar *cl_paused;
extern struct cvar *com_sv_running;
extern struct cvar *com_cl_running;
extern struct cvar *com_introplayed;
extern struct cvar *com_animcheck;
extern struct cvar *com_journal;
extern struct cvar *com_recommendedset;

extern int com_frame_time;

/**
 * @brief Split commandline string into cbuf chunks for processing
 * @param commandline Command line arguments
*/
void com_parse_commandline(char *commandline);

/**
 * @brief Check for "safe" on the command line, which will skip loading config
*/
bool com_safe_mode(void);

/**
 * @brief Searches for command line parameters that are set commands.
 * If match is not NULL, only that cvar will be looked for.
 * That is necessary because cddir and basedir need to be set
 * before the filesystem is started, but all other sets shouls
 * be after execing the config and default.
 * @param match
*/
void com_startup_variable(const char *match);

/**
 * @brief Adds command line parameters as script statements
 * where commands are seperated by + signs
 * @return Returns qtrue if any late commands were added, which
 * will keep the demoloop from immediately starting
*/
bool com_add_startup_commands(void);

/**
 * @brief Check if a string contains a substring
 * @param str1 String to check
 * @param str2 Substring to check for
 * @param casesensitive Whether to do case-sensitive checking
 * @return > 0 if substring is found, NULL otherwise
*/
char *com_string_contains(char *str1, char *str2, bool casesensitive);

/**
 * @brief Filter a string for regex-style matching * ? [ ]
 * @param filter String containing the filter
 * @param name String to search
 * @param casesensitive Whether to do case-senstivie checking
 * @return true if filter matches, false otherwise
*/
bool com_filter(char *filter, char *name, bool casesensitive);

/**
 * @brief Filter a path string for regex-style matching * ? [ ]
 * @param filter String containing the filter
 * @param name String to search
 * @param casesensitive Whether to do case-senstivie checking
 * @return true if filter matches, false otherwise
*/
bool com_filter_path(char *filter, char *name, bool casesensitive);

/**
 * @brief Allocates a new char * for the given string
 * @param in String to copy
 * @return The new string
*/
char *copy_string(const char *in);

/**
 * @brief Get events and push them onto the stack until we get a null event
 * with the current time
 * @return The last event time
*/
int com_milliseconds(void);

/**
 * @brief Write cvars with CVAR_ARCHIVE flag to a config file
 * @param filename Name of config file to write
*/
void com_write_config_to_file(const char *filename);

/**
 * @brief Write all cvar defaults to a config file
 * @param filename Name of config file to write
*/
void com_write_defaults_to_file(const char *filename);

/**
 * @brief Main loop for the game
*/
void com_frame(void);

/**
 * @brief Initialize everything important
 * @param cmdline Command line passed from stdin
*/
void com_init(char *cmdline);

/**
 * @brief Shutdown system
*/
void com_shutdown(void);

void com_read_cdkey(const char *filename);
void com_append_cdkey(const char *filename);

//
//
// common/events.c
//
//

/**
 * @brief Initialize journaling
*/
void com_init_journaling(void);

/**
 * @brief Initialize `com_pushed_events` stack
*/
void com_init_push_event(void);

/**
 * @brief Push the given event into the stack
*/
void com_push_event(struct system_event *event);

/**
 * @brief Handles getting "real events" including from journalfiles
 * @return An event, or an empty event if nothing was found
*/
struct system_event com_get_real_event(void);

/**
 * @brief Gets an event either from the event stack, or from com_get_real_event
 * @return An event, or an empty event if nothing was found
*/
struct system_event com_get_event(void);

/**
 * @brief Main event loop, gets new events and processes them, until all done
 * @return The last event time
*/
int com_event_loop(void);

//
//
// common/fields.c
//
//
void field_clear(struct field *edit);
void field_complete_command(struct field *edit);

#endif // __COMMON_GLOBAL_H__
