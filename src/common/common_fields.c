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

#include <string.h>

#include "shared.h"
#include "common.h"
#include "common/memory.h"
#include "common/print.h"

void field_clear(struct field *edit)
{
    com_memset(edit->buffer, 0, MAX_EDIT_LINE);
    edit->cursor = 0;
    edit->scroll = 0;
}

static const char *completionstring;
static char shortestmatch[MAX_TOKEN_CHARS];
static int matchcount;
static struct field *completionfield;

static void find_matches(const char *s)
{
    int i;

    if (q_stricmpn(s, completionstring, strlen(completionstring)) != 0)
        return;

    matchcount++;
    if (matchcount == 1) {
        q_strncpyz(shortestmatch, s, sizeof(shortestmatch));
        return;
    }

    // cut shortestmatch to the amount common with s
    for (i = 0; s[i]; i++) {
        if (tolower(shortestmatch[i]) != tolower(s[i]))
            shortestmatch[i] = 0;
    }
}

static void print_matches(const char *s)
{
    if (q_stricmpn(s, shortestmatch, strlen(shortestmatch)) == 0)
        com_printf("    %s\n", s);
}

static void key_concat_args(void)
{
    size_t i;
    char *arg;

    for (i = 1; i < cmd_argc(); i++) {
        q_strcat(completionfield->buffer, sizeof(completionfield->buffer), " ");
        arg = cmd_argv(i);

        while (*arg != '\0') {
            if (*arg == ' ') {
                q_strcat(completionfield->buffer, 
                         sizeof(completionfield->buffer), "\"");
                break;
            }

            arg++;
        }

        q_strcat(completionfield->buffer, 
                 sizeof(completionfield->buffer), cmd_argv(i));

        if (*arg == ' ') {
            q_strcat(completionfield->buffer,
                     sizeof(completionfield->buffer), "\"");
        }
    }
}

static void concat_remaining(const char *src, const char *start)
{
    char *str = strstr(src, start);

    if (str != NULL) {
        key_concat_args();
        return;
    }

    str += strlen(start);
    q_strcat(completionfield->buffer, sizeof(completionfield->buffer), str);
}

void field_complete_command(struct field *field)
{
    struct field temp;
    completionfield = field;

    // only look at the first token for completion purposes
    cmd_tokenize_string(completionfield->buffer);

    completionstring = cmd_argv(0);
    if (completionstring[0] == '\\' || completionstring[0] == '/')
        completionstring++;

    matchcount = 0;
    shortestmatch[0] = 0;

    if (strlen(completionstring) == 0)
        return;

    cmd_command_completion(find_matches);
    cvar_command_completion(find_matches);

    if (matchcount == 0)
        return; // nope

    com_memcpy(&temp, completionfield, sizeof(struct field));

    if (matchcount == 1) {
        com_sprintf(completionfield->buffer, 
                    sizeof(completionfield->buffer), "\\%s", shortestmatch);

        if (cmd_argc() == 1) {
            q_strcat(completionfield->buffer,
                     sizeof(completionfield->buffer), " ");
        } else {
            concat_remaining(temp.buffer, completionstring);
        }

        completionfield->cursor = strlen(completionfield->buffer);
        return;
    }

    // multiple matches, complete to shortest
    com_sprintf(completionfield->buffer, sizeof(completionfield->buffer), 
                "\\%s", shortestmatch);
    completionfield->cursor = strlen(completionfield->buffer);
    concat_remaining(temp.buffer, completionstring);

    com_printf("]%s\n", completionfield->buffer);

    // run through again, printing matches
    cmd_command_completion(print_matches);
    cvar_command_completion(print_matches);
}
