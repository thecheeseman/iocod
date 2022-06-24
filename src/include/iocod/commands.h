/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

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

#ifndef IC_COMMANDS_H
#define IC_COMMANDS_H

#include "iocod.h"

/**
 * @defgroup commands Commands
 * @brief Command module for handling the command buffer and command line
 * interaction.
 * @{
 */

enum cbuf_exec_when {
    EXEC_NOW,
    EXEC_INSERT,
    EXEC_APPEND
};

/**
 * @brief Initialize command buffer.
*/
IC_PUBLIC
void cbuf_init(void);

/**
 * @brief Add the given @p text to the end of the command buffer. Does not add
 * a trailing `\n`.
 * @param[in] text text to add to the command buffer
 * @return true on success, false on failure
*/
IC_PUBLIC
bool cbuf_add_text(const char *text);

/**
 * @brief Insert the given @p text immediately after the current command. Will
 * add a trailing `\n`.
 * @param[in] text text to insert
 * @return true on success, false on failure
*/
IC_PUBLIC
bool cbuf_insert_text(const char *text);

/**
 * @brief Execute the given @p text at @p when time.
 * @param[in] when when to execute the text
 * @param[in] text text to execute
 * @return true on success, false on failure
*/
IC_PUBLIC
bool cbuf_execute_text(int when, const char *text);

/**
 * @brief Execute whatever is currently in the command buffer up to a `\n`
 * terminated line.
 * @return true on success, false on failure
*/
IC_PUBLIC
bool cbuf_execute(void);

/**
 * @brief Command function structure.
*/
struct cmd_function {
    /**
     * @brief Next command in the linked list.
    */
    struct cmd_function *next;

    /**
     * @brief Name of the command.
    */
    char *name;

    /**
     * @brief Function to call when the command is executed.
    */
    void (*function)(void);

    // completionfunc complete;
};

/**
 * @brief Initialize command system.
 */
IC_PUBLIC
void cmd_init(void);

/**
 * @brief Get the current number of command line arguments.
 * @return the number of command line arguments
*/
IC_PUBLIC
unsigned int cmd_argc(void);

/**
 * @brief Get the specified command line argument @p arg.
 * @param[in] arg the index of the argument to get
 * @return the argument or "" if @p arg is out of range
*/
IC_PUBLIC
char *cmd_argv(unsigned int arg);

/**
 * @brief Get the current command line argument and copy it to a string buffer.
 * @param[in]  arg    the index of the argument to get
 * @param[in]  buflen the length of the buffer
 * @param[out] buf    the buffer to store the argument in
*/
IC_PUBLIC
void cmd_argv_buffer(unsigned int arg, size_t buflen, char *buf);

/**
 * @brief Get all command line arguments as a single string from starting 
 * argument @p arg.
 * @param[in] arg the index of the starting argument
 * @return the command line arguments as a single string or "" if @p arg is 
 * out of range
*/
IC_PUBLIC
char *cmd_args_from(unsigned int arg);

/**
 * @brief Get all command line arguments as a single string. Starts from 
 * argument index 1.
 * @return the command line arguments as a single string or "" if there are no
 * arguments
*/
IC_PUBLIC
char *cmd_args(void);

/**
 * @brief Get all command line arguments as a single string and copy it to a 
 * string buffer.
 * @param[in]  buflen the length of the buffer
 * @param[out] buf    the buffer to store the arguments in
*/
IC_PUBLIC
void cmd_args_buffer(size_t buflen, char *buf);

/**
 * @brief Parse a single line of text into arguments and executes as if it was
 * entered in the command line.
 * @param[in] text the text to execute
 * @return true on success, false on failure
*/
IC_PUBLIC
bool cmd_execute_string(const char *text);

/**
 * @brief Breaks up the command line into argument tokens.
 * @param[in] text_in the text to tokenize
*/
IC_PUBLIC
void cmd_tokenize_string(const char *text_in);

/**
 * @brief Add the given command @p name to the command list.
 * @param[in] name     the name of the command
 * @param[in] function the function to call when the command is executed
 * @return true on success, false on failure
*/
IC_PUBLIC
bool cmd_add(const char *name, void (*function)(void));

/**
 * @brief Remove the given @p name command from the command list.
 * @param[in] name the name of the command to remove
 * @return true on success, false on failure
*/
IC_PUBLIC
bool cmd_remove(const char *name);

/**
 * @brief Find the given @p name command.
 * @param[in] name the name of the command to find
 * @return the command or NULL if not found
*/
IC_PUBLIC
struct cmd_function *cmd_find(const char *name);

/** @} */

#endif /* IC_COMMANDS_H */
