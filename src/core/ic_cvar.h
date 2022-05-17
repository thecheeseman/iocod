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
 * @defgroup cvar Cvars
 * @brief C Variables.
 * @{ 
 */

#ifndef IC_CVAR_H
#define IC_CVAR_H

/**
 * @def MAX_CVARS
 * @brief Maximum number of cvars allowed by the system.
 * 
 * RTCW/COD have this set to 1024 by default.
 */
#define MAX_CVARS 4096

/**
 * @def MAX_CVAR_VALUE_STRING
 * @brief Maximum length for vmcvar strings.
 */
#define MAX_CVAR_VALUE_STRING 256

/**
 * @enum cvar_flags
*/
enum cvar_flags {
    CVAR_NONE           = 0,    /**< no flags */
    CVAR_ARCHIVE        = 1,    /**< used for system variables, not for
                                     player-specific configurations */
    CVAR_USER_INFO      = 2,    /**< sent to server on connect or change */
    CVAR_SERVER_INFO    = 4,    /**< sent in response to packet requests */
    CVAR_SYSTEM_INFO    = 8,    /**< duplicated on all clients */
    CVAR_INIT           = 16,   /**< can only be changed from the command line */
    CVAR_LATCH          = 32,   /**< only updated when `cvar_get()` is called */
    CVAR_ROM            = 64,   /**< display only */
    CVAR_USER_CREATED   = 128,  /**< created by the 'set' family of commands */
    CVAR_TEMP           = 256,  /**< can be set even when cheats are disabled,
                                     but is not archived */
    CVAR_CHEAT          = 512,  /**< cannot be changed if cheats are disabled */
    CVAR_NO_RESTART     = 1024, /**< not cleared when `cvar_restart` is run */
    CVAR_WOLF_INFO      = 2048, /**< added in RTCW */
    CVAR_4096           = 4096, /**< unknown */
    CVAR_8192           = 8192  /**< added in COD1 */
};

/**
 * @brief Cvar floating point type.
 * @note All Q3-based games use `float`.
*/
typedef double  cvar_float;

/**
 * @brief Cvar integer type.
 * @note All Q3-based games use `int`.
*/
typedef long    cvar_int;

/**
 * @brief Cvar structure.
 * 
 * @warning Nothing outside of the `cvar_*()` functions should modify
 * these fields!
*/
struct cvar {
    char *name;                 /**< cvar name */
    
    char *string;               /**< string value */
    char *reset_string;         /**< `cvar_restart` resets to this value */
    char *latched_string;       /**< used for `CVAR_LATCH` flagged cvars */
    
    enum cvar_flags flags;      /**< flags */
    
    bool modified;              /**< set each time the cvar is changed */
    int modification_count;     /**< incremented each time the cvar is changed */

    cvar_float value;           /**< floating point representation of string */
    cvar_int integer;           /**< integer representation of string */

    struct cvar *next;          /**< next cvar */
    struct cvar *hash_next;     /**< next hashtable entry */
};

/**
 * @brief Cvar structure for virtual machines. 
 *
 * VM's can't access `struct cvar` directly, so they must ask for structured
 * updates via this interface. 
*/
struct vmcvar {
    int handle;                 /**< cvar handle */
    int modification_count;     /**< incremented each time the cvar is changed */

    cvar_float value;           /**< floating point representation of string */
    cvar_int integer;           /**< integer representation of string */
    char string[MAX_CVAR_VALUE_STRING]; /**< string value */
};

/**
 * @brief Validate a cvar string
 * @param[in] s string to validate
 * @return false if string contains '\' or '"' or ';' chars, true otherwise
*/
IC_PUBLIC
IC_NON_NULL(1)
bool cvar_validate_string(const char *s);

/**
 * @brief Return a cvar's string value
 * @param[in] var_name name of the cvar to search for
 * @return a string containing the value
*/
IC_PUBLIC
IC_NON_NULL(1)
char *cvar_variable_string(const char *var_name);

/**
 * @brief Return a cvar's string value into the given string buffer
 * @param[in]  var_name name of the cvar to search for
 * @param[out] buffer   pointer to string buffer
 * @param[in]  bufsize  size of the string buffer
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
void cvar_variable_string_buffer(const char *var_name, char *buffer,
                                 int bufsize);

/**
 * @brief Some cvar values need to be safe from foreign characters
 * @param[in] value value to clean
 * @return a cleaned string
 */
IC_PUBLIC
IC_NON_NULL(1)
char *cvar_clean_foreign_characters(const char *value);

/**
 * @brief Return a cvar's floating point value
 * @param[in] var_name name of the cvar to search for
 * @return a float containing the value
*/
IC_PUBLIC
IC_NON_NULL(1)
cvar_float cvar_variable_value(const char *var_name);

/**
 * @brief Return a cvar's integer value
 * @param[in]   var_name name of the cvar to search for
 * @return an integer containing the value
*/
IC_PUBLIC
IC_NON_NULL(1)
cvar_int cvar_variable_integer_value(const char *var_name);

/** @} */

#endif /* IC_CVAR_H */
