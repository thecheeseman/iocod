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

#ifndef CONFIG_LOCAL_H
#define CONFIG_LOCAL_H

#include "iocod.h"
#include <ctype.h>
#include <stdlib.h>

/**
 * @defgroup conf_local Config (local)
 * @brief Local config procedures and structures needed for config module.
 * @{
 */

/**
 * @brief Error codes for conf errors that can occur.
*/
enum conf_error {
    /**
     * @brief No errors.
    */
    CONF_ERR_OK,

    /**
     * @brief "NULL conf pointer".
    */
    CONF_ERR_NULL_CFG,

    /**
     * @brief "NULL filename".
    */
    CONF_ERR_NULL_FILENAME,

    /**
     * @brief "NULL options pointer".
    */
    CONF_ERR_NULL_OPTIONS,

    /**
     * @brief "Error while stat(): %s". Filled out with strerror(errno).
    */
    CONF_ERR_STAT,

    /**
     * @brief "Error while fopen(): %s". Filled out with strerror(errno).
    */
    CONF_ERR_FOPEN,

    /**
     * @brief "Error while fread(): %s". Filled out with strerror(errno).
    */
    CONF_ERR_FREAD,

    /**
     * @brief "Line %d is incomplete". Parse error.
    */
    CONF_ERR_INCOMPLETE,

    /**
     * @brief "Token too large on line %d". Parse error, filled with line #.
    */
    CONF_ERR_TOKEN_TOO_LARGE,

    /**
     * @brief "Expected value for token '%s'". Parse error (config read).
    */
    CONF_ERR_EXPECTED_TOKEN,

    /**
     * @brief "Unknown value for boolean: %s". Wrong boolean string.
    */
    CONF_ERR_UNKNOWN_BOOLEAN,

    /**
     * @brief "Unknown config variable type %d".
    */
    CONF_ERR_UNKNOWN_TYPE,

    CONF_ERR_UNKNOWN
};

/**
 * @brief Set an error state so we can let the user know.
 * @param[in] err errcode.
 * @param[in] ... optional arguments
*/
void conf_set_error(int err, ...);

/**
 * @brief Get the last error string.
 * @return error string
*/
char *conf_error_string(void);

/**
 * @brief Load a config file and read data into memory.
 * @param[in] cfg config file
 * @return true if success, false otherwise
*/
qbool conf_load(struct conf *cfg);

/**
 * @brief Set config's defaults. Allocates memory if necessary for strings.
 * @param[in] cfg config file
 * @return true if success, false otherwise
*/
qbool conf_set_defaults(struct conf *cfg);

/**
 * @brief Write defaults out to config file if it doesn't exist.
 * @param[in] cfg config file
 * @return true if success, false otherwise
*/
qbool conf_write_defaults(struct conf *cfg);

/**
 * @brief Parse loaded config data into the config structure.
 * @param[in] cfg config file
 * @return true if success, false otherwise
*/
qbool conf_parse(struct conf *cfg);

/** @} */

#endif /* CONFIG_LOCAL_H */
