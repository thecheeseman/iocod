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
#include "common/error.h"
#include "common/memory.h"
#include "common/print.h"
#include "stringlib.h"

// multiple character punctuation tokens
static const char *punctuation[] = {
    "+=", "-=",  "*=",  "/=", "&=", "|=", "++", "--",
    "&&", "||",  "<=",  ">=", "==", "!=",
    NULL
};

/**
 * @brief 
*/
struct parse_info {
    char token[MAX_TOKEN_CHARS];

    int lines;

    bool unget_token;

    bool field_0x408;	
    bool field_0x40c;

    int field_0x410;

    int old_lines;
    char *buf_p;
    char parse_file[MAX_QPATH];
};

#define MAX_PARSE_INFO 16
static struct parse_info parse_info[MAX_PARSE_INFO];
static int parse_info_num;
static struct parse_info *pi = &parse_info[0];

void com_begin_parse_session(const char *name)
{
    if (parse_info_num == MAX_PARSE_INFO - 1)
        com_error(ERR_FATAL, "session overflow");

    parse_info_num++;
    pi = &parse_info[parse_info_num];

    pi->lines = 1;
    pi->unget_token = false;

    pi->field_0x408 = true;
    pi->field_0x40c = false;

    q_strncpyz(pi->parse_file, name, sizeof(pi->parse_file));
}

void com_end_parse_session(void)
{
    if (parse_info_num == 0)
        com_error(ERR_FATAL, "session underflow");

    parse_info_num--;
    pi = &parse_info[parse_info_num];
}

void com_reset_parse_session(void)
{
    pi = &parse_info[0];
    parse_info_num = 0;
}

void com_set_field_0x40c(bool val)
{
    pi->field_0x40c = val;
}

int com_get_current_parse_line(void)
{
    return pi->lines;
}

void com_script_error(const char *msg, ...)
{
    va_list argptr;
    char string[32000];

    va_start(argptr, msg);
    vsnprintf(string, sizeof(string), msg, argptr);
    va_end(argptr);

    com_error(ERR_DROP, "File %s, line %i: %s", pi->parse_file, pi->lines, 
              string);
}

void com_script_warning(const char *msg, ...)
{
    va_list argptr;
    char string[32000];

    va_start(argptr, msg);
    vsnprintf(string, sizeof(string), msg, argptr);
    va_end(argptr);

    com_printf("File %s, line %i: %s", pi->parse_file, pi->lines, string);
}

void com_unget_token(void)
{
    if (pi->unget_token)
        com_script_error("unget_token called twice");

    pi->unget_token = true;
}

static char *skip_whitespace(char *data, bool *has_new_lines)
{
    int c;

    while ((c = *data) <= ' ') {
        if (!c)
            return NULL;

        if (c == '\n') {
            pi->lines++;
            *has_new_lines = true;
        }

        data++;
    }

    return data;
}

static char *com_parse_ext2(char **data_p, bool allow_line_breaks)
{
    char *c = *data_p;

    pi->token[0] = '\0';

    if (!allow_line_breaks) {
        if (*c == '\r' || *c == '\n')
            return pi->token;
    } else {
        for (; (*c == '\r' || (*c == '\n')); c++);
    }

    return NULL;
}

static char *com_parse_ext(char **data_p, bool allow_line_breaks)
{
    int c = 0, len;
    bool has_new_lines = false;
    char *data;
    const char **punc;

    if (!data_p)
        com_error(ERR_FATAL, "NULL data_p");

    data = *data_p;
    len = 0;
    pi->token[0] = 0;

    // make sure incoming data is valid
    if (!data) {
        *data_p = NULL;
        return pi->token;
    }

    pi->old_lines = pi->lines;
    pi->buf_p = *data_p;

    if (pi->field_0x40c)
        return com_parse_ext2(data_p, allow_line_breaks);

    // skip any leading whitespace
    while (true) {
        // skip whitespace
        data = skip_whitespace(data, &has_new_lines);
        if (!data) {
            *data_p = NULL;
            return pi->token;
        }

        if (has_new_lines && !allow_line_breaks) {
            *data_p = data;
            return pi->token;
        }

        c = *data;

        // skip double slash comments
        if (c == '/' && data[1] == '/') {
            while (*data && *data != '\n')
                data++;

            continue;
        }

        // skip /* */ comments
        if (c == '/' && data[1] == '*') {
            while (*data && (*data != '*' || data[1] != '/')) {
                if (*data == '\n')
                    pi->lines++;

                data++;
            }

            if (*data)
                data += 2;

            continue;
        }

        // a real token to parse
        break;
    }

    // handle quoted strings
    if (c == '\"') {
        data++;

        while (true) {
            c = *data++;

            if ((c == '\\') && (*data == '\"')) {
                // allow quoted strings to use \" to indicate the " character
                data++;
            } else if (c == '\"' || !c) {
                pi->token[len] = 0;
                *data_p = (char *) data;
                return pi->token;
            } else if (*data == '\n') {
                pi->lines++;
            }
            if (len < MAX_TOKEN_CHARS - 1) {
                pi->token[len] = c;
                len++;
            }
        }
    }

    // check for a number
    // is this parsing of negative numbers going to cause expression problems
    if ((c >= '0' && c <= '9') || (c == '-' && data[1] >= '0' && data[1] <= '9') ||
        (c == '.' && data[1] >= '0' && data[1] <= '9')) {
        do {

            if (len < MAX_TOKEN_CHARS - 1) {
                pi->token[len] = c;
                len++;
            }
            data++;

            c = *data;
        } while ((c >= '0' && c <= '9') || c == '.');

        // parse the exponent
        if (c == 'e' || c == 'E') {
            if (len < MAX_TOKEN_CHARS - 1) {
                pi->token[len] = c;
                len++;
            }

            data++;
            c = *data;

            if (c == '-' || c == '+') {
                if (len < MAX_TOKEN_CHARS - 1) {
                    pi->token[len] = c;
                    len++;
                }
                data++;
                c = *data;
            }

            do {
                if (len < MAX_TOKEN_CHARS - 1) {
                    pi->token[len] = c;
                    len++;
                }
                data++;

                c = *data;
            } while (c >= '0' && c <= '9');
        }

        if (len == MAX_TOKEN_CHARS) {
            len = 0;
        }
        pi->token[len] = 0;

        *data_p = (char *) data;
        return pi->token;
    }

    // check for a regular word
    // we still allow forward and back slashes in name tokens for pathnames
    // and also colons for drive letters
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || 
        c == '/' || c == '\\') {
        do {
            if (len < MAX_TOKEN_CHARS - 1) {
                pi->token[len] = c;
                len++;
            }
            data++;

            c = *data;
        } while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'
                 || (c >= '0' && c <= '9') || c == '/' || c == '\\' 
                 || c == ':' || c == '.');

        if (len == MAX_TOKEN_CHARS)
            len = 0;

        pi->token[len] = 0;

        *data_p = (char *) data;
        return pi->token;
    }

    // check for multi-character punctuation token
    for (punc = punctuation; *punc; punc++) {
        int	l;
        int	j;

        l = strlen(*punc);
        for (j = 0; j < l; j++) {
            if (data[j] != (*punc)[j])
                break;
        }

        if (j == l) {
            // a valid multi-character punctuation
            com_memcpy(pi->token, *punc, l);
            pi->token[l] = 0;
            data += l;
            *data_p = (char *) data;
            return pi->token;
        }
    }

    // single character punctuation
    pi->token[0] = *data;
    pi->token[1] = 0;
    data++;
    *data_p = (char *) data;

    return pi->token;
}

char *com_parse(char **buf_p)
{
    if (pi->unget_token) {
        pi->unget_token = false;
        *buf_p = pi->buf_p;
        pi->lines = pi->old_lines;
    }

    return com_parse_ext(buf_p, true);
}

char *com_parse_on_line(char **buf_p)
{
    if (pi->unget_token) {
        pi->unget_token = false;

        if (!pi->field_0x408)
            return pi->token;

        *buf_p = pi->buf_p;
        pi->lines = pi->old_lines;
    }

    return com_parse_ext(buf_p, false);
}

void com_match_token(char **buf_p, char *match, bool warning)
{
    char *token = com_parse(buf_p);

    if (strcmp(token, match)) {
        if (warning)
            com_script_warning("match_token: %s != %s", token, match);
        else
            com_script_error("match_token: %s != %s", token, match);
    }
}

void com_skip_braced_section(char **program) 
{
    const char *token;
    int depth;

    depth = 0;
    do {
        token = com_parse(program);

        if (token[1] == 0) {
            if (token[0] == '{')
                depth++;
            else if (token[0] == '}')
                depth--;
        }
    } while (depth && *program);
}

void com_skip_rest_of_line(char **data_p)
{
    char *p;
    char c;

    p = *data_p;
    
    while ((c = *p++) != 0) {
        if (c == '\n') {
            pi->lines++;
            break;
        }
    }

    *data_p = p;
}

char *com_parse_rest_of_line(char **data_p) 
{
    static char	line[MAX_TOKEN_CHARS];
    const char *token;

    line[0] = 0;
    while (1) {
        token = com_parse_on_line(data_p);
        if (!token[0])
            break;

        if (line[0])
            q_strcat(line, sizeof(line), " ");

        q_strcat(line, sizeof(line), token);
    }

    return line;
}

float com_parse_float(char **buf_p) 
{
    const char *token;

    token = com_parse(buf_p);
    if (!token[0])
        return 0;

    return atof(token);
}

int com_parse_int(char **buf_p) 
{
    const char *token;

    token = com_parse(buf_p);
    if (!token[0])
        return 0;
    
    return atoi(token);
}

void com_parse_1d_matrix(char **buf_p, int x, float *m) 
{
    const char *token;
    int	i;

    com_match_token(buf_p, "(", false);

    for (i = 0; i < x; i++) {
        token = com_parse(buf_p);
        m[i] = atof(token);
    }

    com_match_token(buf_p, ")", false);
}

void com_parse_2d_matrix(char **buf_p, int y, int x, float *m) 
{
    int		i;

    com_match_token(buf_p, "(", false);

    for (i = 0; i < y; i++)
        com_parse_1d_matrix(buf_p, x, m + i * x);

    com_match_token(buf_p, ")", false);
}

void com_parse_3d_matrix(char **buf_p, int z, int y, int x, float *m) 
{
    int	i;

    com_match_token(buf_p, "(", false);

    for (i = 0; i < z; i++)
        com_parse_2d_matrix(buf_p, y, x, m + i * x * y);

    com_match_token(buf_p, ")", false);
}
