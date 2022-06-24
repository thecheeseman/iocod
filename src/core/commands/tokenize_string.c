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

#include "cmd_local.h"

char cmd_tokenized[INFO_STRING_BIG + MAX_STRING_TOKENS];
char cmd_cmd[INFO_STRING_BIG];

IC_PUBLIC
void cmd_tokenize_string(const char *text_in)
{
    argc = 0;

    if (text_in == NULL)
        return;

    strncpyz(cmd_cmd, text_in, sizeof(cmd_cmd));

    const char *text = text_in;
    char *textout = cmd_tokenized;

    while (true) {
        if (argc == MAX_STRING_TOKENS)
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

                text += 2;
            } else {
                break; // ready to parse
            }
        }

        // handle quoted strings
        if (*text == '"') {
            argv[argc] = textout;
            argc++;
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
        argv[argc] = textout;
        argc++;

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
