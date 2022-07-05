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

/**
 * @defgroup parse Parse Tools
 * @brief Tools for parsing configs, etc.
 * @{
 */

#ifndef IC_PARSE_H
#define IC_PARSE_H

#include "iocod/platform.h"
#include "iocod/types.h"

#define MAX_TOKEN_CHARS 1024

/**
 * @brief Parse info structure.
*/
struct parse_info {
    char token[MAX_TOKEN_CHARS];    /**< current token */

    size_t lines;                   /**< current line number */
    size_t old_lines;               /**< used to save line number for 
                                         line crossing */

    bool unget_token;               /**< if true, will jump back one token */

    char *buf_p;                    /**< pointer to actual data */
    char parse_file[256];           /**< file name */
};

/*
 *
 * ps_error.c
 * 
 */

/**
 * @brief Generate a script warning. Ultimately calls `ic_warning()`.
 * 
 * @param[in] fmt `printf`-like string format
 * @param[in] ... varargs
*/
IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void ps_script_warning(const char *fmt, ...);

/**
 * @brief Generate a script warning. Ultimately calls `ic_error()`.
 *
 * @param[in] fmt `printf`-like string format
 * @param[in] ... varargs
*/
IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void ps_script_error(const char *fmt, ...);

/*
 *
 * ps_parse.c
 *
 */

/**
 * @brief Parse given @p data until a token is found. Ignores newlines.
 * 
 * Automatically ignores C++-style comments (`//`) and C-style comments.
 * Valid tokens are integers, decimals (with optional +E notation)
 * strings (both \"quoted\" and unquoted).
 * 
 * @param[in] buf_p data to parse from
 * 
 * @return a token, or NULL if failed
*/
IC_PUBLIC
IC_NON_NULL(1)
char *ps_parse(char **buf_p);

/**
 * @brief Parse given @p data until a token is found, stopping if a new line
 * is encountered.
 *
 * @param[in] buf_p data to parse from
 * 
 * @see ps_parse
 *
 * @return a token, or NULL if failed
*/
IC_PUBLIC
IC_NON_NULL(1)
char *ps_parse_on_line(char **buf_p);

/**
 * @brief Match a given token.
 *
 * @param[in] buf_p   data to parse from
 * @param[in] match   token string to match
 * @param[in] warning if true, will only generate a warning instead of an error
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
void ps_match_token(char **buf_p, char *match, bool warning);

/**
 * @brief Skip an entire braced section. Automatically handles any nested 
 * braced sections.
 *
 * @param[in] buf_p data to parse from
*/
IC_PUBLIC
IC_NON_NULL(1)
void ps_skip_braced_section(char **buf_p);

/**
 * @brief Skips the rest of the line until a new line is encountered.
 *
 * @param[in] data_p data to parse from
*/
IC_PUBLIC
IC_NON_NULL(1)
void ps_skip_rest_of_line(char **data_p);

/**
 * @brief Continue parsing the rest of the current line.
 *
 * @param[in] data_p data to parse from
 *
 * @return a token, or NULL if failed
*/
IC_PUBLIC
IC_NON_NULL(1)
char *ps_parse_rest_of_line(char **data_p);

/**
 * @brief Try to parse the next token as a decimal value.
 *
 * @param[in] buf_p data to parse from
 *
 * @return 0.0 if failed, actual value otherwise
*/
IC_PUBLIC
IC_NON_NULL(1)
double ps_parse_value(char **buf_p);

/**
 * @brief Try to parse the next token as an integer value.
 *
 * @param[in] buf_p data to parse from
 *
 * @return 0 if failed, actual value otherwise
*/
IC_PUBLIC
IC_NON_NULL(1)
long ps_parse_integer(char **buf_p);

/**
 * @brief Try to parse a 1-dimensional matrix inside parentheticals `(` and `)`.
 * 
 * Can handle any arbitrary matrix size, but generally use to parse vectors.
 * For example:
 * @code
 * // this set of tokens
 * (42 99 77)
 * // can be parsed with this:
 * vec3_t vec;
 * ps_parse_1d_matrix(&data, 3, vec);
 * @endcode
 *
 * @param[in] buf_p data to parse from
 * @param[in] x     size of matrix
 * @param[out] m    pointer to matrix
*/
IC_PUBLIC
IC_NON_NULL(1, 3)
void ps_parse_1d_matrix(char **buf_p, size_t x, vec_t *m);

/**
 * @brief Try to parse a 2-dimensional matrix inside parentheticals `(` and `)`.
 *
 * Can handle any arbitrary matrix size, for example a 2x3 matrix:
 * `((42 99 77) (55 66 100))`
 *
 * @param[in] buf_p data to parse from
 * @param[in] y     size of matrix Y dimension
 * @param[in] x     size of matrix X dimension
 * @param[out] m    pointer to matrix
*/
IC_PUBLIC
IC_NON_NULL(1, 4)
void ps_parse_2d_matrix(char **buf_p, size_t y, size_t x, vec_t *m);

/**
 * @brief Try to parse a 3-dimensional matrix inside parentheticals `(` and `)`.
 *
 * @param[in] buf_p data to parse from
 * @param[in] z     size of matrix Z dimension
 * @param[in] y     size of matrix Y dimension
 * @param[in] x     size of matrix X dimension
 * @param[out] m    pointer to matrix
*/
IC_PUBLIC
IC_NON_NULL(1, 5)
void ps_parse_3d_matrix(char **buf_p, size_t z, size_t y, size_t x, vec_t *m);

/**
 * @brief Rolls back one token, so the next call to `ps_parse()`
 * gets the previous token.
*/
IC_PUBLIC
void ps_unget_token(void);

/*
 *
 * ps_session.c
 *
 */

/**
 * @brief Begin a new session.
 * 
 * @param[in] name name of session
*/
IC_PUBLIC
IC_NON_NULL(1)
void ps_begin_session(const char *name);

/**
 * @brief End current session.
*/
IC_PUBLIC
void ps_end_session(void);

/**
 * @brief Reset all sessions.
*/
IC_PUBLIC
void ps_reset(void);

/** @} */

#endif /* IC_PARSE_H */
