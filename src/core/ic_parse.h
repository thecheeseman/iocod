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
 * @defgroup parse Parse Tools
 * @brief Tools for parsing configs, etc.
 * @{
 */

#ifndef IC_PARSE_H
#define IC_PARSE_H

#include "iocod.h"

#define MAX_TOKEN_CHARS 1024

struct parse_info {
    char token[MAX_TOKEN_CHARS];

    size_t lines;
    size_t old_lines;

    bool unget_token;

    char *buf_p;
    char parse_file[256];
};

/*
 *
 * ps_error.c
 * 
 */

IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void ps_script_warning(const char *fmt, ...);

IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void ps_script_error(const char *fmt, ...);

/*
 *
 * ps_parse.c
 *
 */

IC_PUBLIC
IC_NON_NULL(1)
char *ps_parse(char **buf_p);

IC_PUBLIC
IC_NON_NULL(1)
char *ps_parse_on_line(char **buf_p);

IC_PUBLIC
IC_NON_NULL(1, 2)
void ps_match_token(char **buf_p, char *match, bool warning);

IC_PUBLIC
IC_NON_NULL(1)
void ps_skip_braced_section(char **buf_p);

IC_PUBLIC
IC_NON_NULL(1)
void ps_skip_rest_of_line(char **data_p);

IC_PUBLIC
IC_NON_NULL(1)
char *ps_parse_rest_of_line(char **data_p);

IC_PUBLIC
IC_NON_NULL(1)
double ps_parse_value(char **buf_p);

IC_PUBLIC
IC_NON_NULL(1)
long ps_parse_integer(char **buf_p);

IC_PUBLIC
IC_NON_NULL(1, 3)
void ps_parse_1d_matrix(char **buf_p, size_t x, vec_t *m);

IC_PUBLIC
IC_NON_NULL(1, 4)
void ps_parse_2d_matrix(char **buf_p, size_t y, size_t x, vec_t *m);

IC_PUBLIC
IC_NON_NULL(1, 5)
void ps_parse_3d_matrix(char **buf_p, size_t z, size_t y, size_t x, vec_t *m);

IC_PUBLIC
void ps_unget_token(void);
/*
 *
 * ps_session.c
 *
 */

IC_PUBLIC
IC_NON_NULL(1)
void ps_begin_session(const char *name);

IC_PUBLIC
void ps_end_session(void);

IC_PUBLIC
void ps_reset(void);

/** @} */

#endif /* IC_PARSE_H */
