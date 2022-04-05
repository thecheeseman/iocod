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

#include "shared.h"
#include "common.h"
#include "common/error.h"
#include "common/memory.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "infostring.h"
#include "strings/stringlib.h"

static size_t _cmd_argc;
static char *_cmd_argv[MAX_STRING_TOKENS];
static char cmd_tokenized[BIG_INFO_STRING + MAX_STRING_TOKENS];
static char cmd_cmd[BIG_INFO_STRING];

struct cmd_function *cmd_functions;

/**
 * @brief Command buffer argument count
 * @param  
 * @return 
*/
size_t cmd_argc(void)
{
    return _cmd_argc;
}

/**
 * @brief Command buffer argument string
 * @param arg 
 * @return 
*/
char *cmd_argv(size_t arg)
{
    if (arg >= _cmd_argc)
        return "";

    return _cmd_argv[arg];
}

/**
 * @brief Command buffer arguments
 * @param  
 * @return 
*/
char *cmd_args(void)
{
    static char cmd_args[MAX_STRING_CHARS];
    size_t i;

    cmd_args[0] = '\0';
    for (i = 1; i < _cmd_argc; i++) {
        strcat(cmd_args, _cmd_argv[i]);

        if (i != _cmd_argc - 1)
            strcat(cmd_args, " ");
    }

    return cmd_args;
}

/**
 * @brief Get command buffer arguments starting from a certain value
 * @param arg 
 * @return 
*/
char *cmd_args_from(size_t arg)
{
    static char cmd_args[BIG_INFO_STRING];
    size_t i;

    cmd_args[0] = '\0';
    for (i = arg; i < _cmd_argc; i++) {
        strcat(cmd_args, _cmd_argv[i]);

        if (i != _cmd_argc - 1)
            strcat(cmd_args, " ");
    }

    return cmd_args;
}

/**
 * @brief Write command argv into a string buffer
 * @param arg 
 * @param buffer 
 * @param bufsize 
*/
void cmd_argv_buffer(size_t arg, char *buffer, size_t bufsize)
{
    strncpyz(buffer, cmd_argv(arg), bufsize);
}

/**
 * @brief Convert input string into tokens
 * @param text_in 
*/
void cmd_tokenize_string(const char *text_in)
{
    const char *text;
    char *textout;

    _cmd_argc = 0;

    if (text_in == NULL)
        return;

    strncpyz(cmd_cmd, text_in, sizeof(cmd_cmd));

    text = text_in;
    textout = cmd_tokenized;

    while (true) {
        if (_cmd_argc == MAX_STRING_TOKENS)
            return;

        while (true) {
            // skip whitespace
            while (*text != '\0' && *text <= ' ')
                text++;

            if (*text == '\0')
                return; // no more tokens

            // skip // comments
            if (text[0] == '/' && text[1] == '/')
                return;

            // skip /* */ comments
            if (text[0] == '/' && text[1] == '*') {
                while (*text != '\0' && (text[0] != '*' || text[1] != '/'))
                    text++;

                if (*text == '\0')
                    return; // no more

                text+= 2;
            } else {
                break; // ready to parse
            }
        }

        // handle quoted strings
        if (*text == '"') {
            _cmd_argv[_cmd_argc] = textout;
            _cmd_argc++;
            text++;

            while (*text != '\0' && *text != '"')
                *textout++ = *text++;

            *textout++ = 0;
            if (*text == '\0')
                return; // no more

            text++;
            continue;
        }

        // regular token
        _cmd_argv[_cmd_argc] = textout;
        _cmd_argc++;

        // skip until whitespace, quote, or command
        while (*text > ' ') {
            if (text[0] == '"')
                break;

            if (text[0] == '/' && text[1] == '/')
                break;

            if (text[0] == '/' && text[1] == '*')
                break;

            *textout++ = *text++;
        }

        *textout++ = 0;

        if (*text == '\0')
            return; // all done
    }
}

/**
 * @brief Add a game/console command 
 * @param cmd_name 
 * @param function 
*/
void cmd_add_command(const char *cmd_name, xcommand function)
{
    struct cmd_function *cmd;

    for (cmd = cmd_functions; cmd != NULL; cmd = cmd->next) {
        if (strcasecmp(cmd_name, cmd->name) == 0) {
            if (function != NULL)
                com_printf("cmd_add_command: %s already defined\n", cmd_name);

            return;
        }
    }

    cmd = z_malloc(sizeof(struct cmd_function));
    cmd->name = copy_string(cmd_name);
    cmd->function = function;

    cmd->alias_count = 0;
    cmd->aliases = z_malloc(sizeof(*cmd->aliases));

    cmd->next = cmd_functions;
    cmd_functions = cmd;
}

/**
 * @brief Remove a game/console command
 * @param cmd_name 
*/
void cmd_remove_command(const char *cmd_name)
{
    int i;
    struct cmd_function *cmd, **back;

    back = &cmd_functions;
    while (true) {
        cmd = *back;

        if (cmd == NULL)
            return;

        if (strcasecmp(cmd_name, cmd->name) == 0) {
            *back = cmd->next;
            if (cmd->name != NULL)
                z_free(cmd->name);
            
            if (cmd->alias_count) {
                for (i = 0; i < cmd->alias_count; i++)
                    z_free(cmd->aliases[i]);
            }
            z_free(cmd->aliases);
            z_free(cmd);
            return;
        }

        back = &cmd->next;
    }
}

/**
 * @brief Add an alias to a command name (e.g. alias "exit" to "quit")
 * 
 * @param cmd_name Command name to alias
 * @param alias The name of the alias to use
*/
void cmd_add_alias(const char *cmd_name, const char *alias)
{
    struct cmd_function *cmd, **prev;
    int c;

    if (cmd_name == NULL || *cmd_name == '\0')
        com_error(ERR_DROP, "tried to add alias with NULL cmd name");

    if (alias == NULL || *alias == '\0')
        com_error(ERR_DROP, "tried to add alias with NULL name");

    for (prev = &cmd_functions; *prev != NULL; prev = &cmd->next) {
        cmd = *prev;

        if (strcasecmp(cmd_name, cmd->name) == 0) {
            // found it
            c = cmd->alias_count;

            cmd->aliases = realloc(cmd->aliases, sizeof(*cmd->aliases) * (c + 1));
            cmd->aliases[c] = z_malloc(sizeof(char *) * (strlen(alias) + 1));
            strcpy(cmd->aliases[c], alias);
            cmd->alias_count++;
        }
    }
}

/**
 * @brief Handle tab-completion
 * 
 * @param callback 
*/
void cmd_command_completion(void (*callback)(const char *s))
{
    struct cmd_function *cmd;

    for (cmd = cmd_functions; cmd != NULL; cmd = cmd->next)
        callback(cmd->name);
}

/**
 * @brief Handle commands
 * 
 * @param text 
*/
void cmd_execute_string(const char *text)
{
    int i;
    bool aliasfound;
    struct cmd_function *cmd, **prev;

    cmd_tokenize_string(text);
    if (cmd_argc() == 0)
        return; // no tokens

    for (prev = &cmd_functions; *prev != NULL; prev = &cmd->next) {
        cmd = *prev;

        // check for any aliases
        aliasfound = false;
        for (i = 0; i < cmd->alias_count; i++) {
            if (strcasecmp(_cmd_argv[0], cmd->aliases[i]) == 0) {
                aliasfound = true;
                break;
            }
        }

        if (aliasfound || strcasecmp(_cmd_argv[0], cmd->name) == 0) {
            // rearrange the links so that the command will be
            // near the head of the list next time it is used
            *prev = cmd->next;
            cmd->next = cmd_functions;
            cmd_functions = cmd;

            // perform the action 
            // let the cgame or game handle it
            if (!cmd->function)
                break;
            else
                cmd->function();
        
            return;
        }
    }

    // check cvars
    if (cvar_command())
        return;

    // client
    if (com_cl_running != NULL && com_cl_running->integer > 0 && cl_game_command())
        return;

    // server
    if (com_sv_running != NULL && com_sv_running->integer > 0 && sv_game_command())
        return;

    // ui
    if (com_cl_running != NULL && com_cl_running->integer > 0 && ui_game_command())
        return;

    // send it as a server command if we are connected
    // this will usually result in a chat message
    cl_forward_command_to_server(text);
}

