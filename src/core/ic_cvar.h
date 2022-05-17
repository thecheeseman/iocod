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

#include "iocod.h"

/**
 * @def MAX_CVARS
 * @brief Maximum number of cvars allowed by the system.
 * 
 * RTCW/COD have this set to 1024 by default.
 */
#define MAX_CVARS 4096

/**
 * @enum cvar_flags
*/
enum cv_flags {
    CV_NONE         = 0,    /**< no flags */
    CV_ARCHIVE      = 1,    /**< used for system variables, not for
                                     player-specific configurations */
    CV_USER_INFO    = 2,    /**< sent to server on connect or change */
    CV_SERVER_INFO  = 4,    /**< sent in response to packet requests */
    CV_SYSTEM_INFO  = 8,    /**< duplicated on all clients */
    CV_INIT         = 16,   /**< can only be changed from the command line */
    CV_LATCH        = 32,   /**< only updated when `cvar_get()` is called */
    CV_ROM          = 64,   /**< display only */
    CV_USER_CREATED = 128,  /**< created by the 'set' family of commands */
    CV_TEMP         = 256,  /**< can be set even when cheats are disabled,
                                     but is not archived */
    CV_CHEAT        = 512,  /**< cannot be changed if cheats are disabled */
    CV_NO_RESTART   = 1024, /**< not cleared when `cvar_restart` is run */
    CV_WOLF_INFO    = 2048, /**< added in RTCW */
    CV_4096         = 4096, /**< unknown */
    CV_8192         = 8192  /**< added in COD1 */
};

/**
 * @brief Cvar floating point type.
 * @note All Q3-based games use `float`.
*/
typedef double  cv_float;

/**
 * @brief Cvar integer type.
 * @note All Q3-based games use `int`.
*/
typedef long    cv_int;

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
    
    enum cv_flags flags;        /**< flags */
    
    bool modified;              /**< set each time the cvar is changed */
    int modification_count;     /**< incremented each time the cvar is changed */

    cv_float value;             /**< floating point representation of string */
    cv_int integer;             /**< integer representation of string */

    struct cvar *next;          /**< next cvar */
    struct cvar *hash_next;     /**< next hashtable entry */
};

/**
 * @def MAX_CVAR_STRING_LEN
 * @brief Maximum length for vmcvar strings.
 */
#define MAX_VMCVAR_STRING_LEN 256

/**
 * @brief Cvar structure for virtual machines. 
 *
 * VM's can't access `struct cvar` directly, so they must ask for structured
 * updates via this interface. 
*/
struct vmcvar {
    int handle;                 /**< cvar handle */
    int modification_count;     /**< incremented each time the cvar is changed */

    cv_float value;             /**< floating point representation of string */
    cv_int integer;             /**< integer representation of string */
    char string[MAX_VMCVAR_STRING_LEN]; /**< string value */
};

/**
 * @brief Find a cvar by name.
 * 
 * @param[in] name name of the cvar to look for
 * @return NULL if not found, otherwise a `struct cvar *`
*/
IC_PUBLIC
struct cvar *cv_find(const char *name);

IC_PUBLIC
struct cvar *cv_get(const char *name, const char *value, enum cv_flags flags);

IC_PUBLIC
struct cvar *cv_set2(const char *name, const char *value, bool force);

/**
 * @brief Some cvar values need to be safe from foreign characters
 * @param[in] value value to clean
 * @return a cleaned string
 */
IC_PUBLIC
char *cv_clear_foreign_chars(const char *value);

/**
 * @brief Return a cvar's string value
 * @param[in] name name of the cvar to search for
 * @return a string containing the value
*/
IC_PUBLIC
IC_NON_NULL(1)
char *cv_string(const char *name);

/**
 * @brief Return a cvar's string value into the given string buffer
 * @param[in]  name  name of the cvar to search for
 * @param[out] buf   pointer to string buffer
 * @param[in]  size  size of the string buffer
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
void cv_string_buffer(const char *name, char *buf, size_t size);

/**
 * @brief Validate a cvar string
 * @param[in] s string to validate
 * @return false if string contains '\' or '"' or ';' chars, true otherwise
*/
IC_PUBLIC
bool cv_validate_string(const char *s);

/**
 * @brief Return a cvar's floating point value
 * @param[in] var_name name of the cvar to search for
 * @return a float containing the value
*/
IC_PUBLIC
IC_NON_NULL(1)
cv_float cv_value(const char *name);

/**
 * @brief Return a cvar's integer value
 * @param[in]   var_name name of the cvar to search for
 * @return an integer containing the value
*/
IC_PUBLIC
IC_NON_NULL(1)
cv_int cv_integer(const char *name);

/** @} */

#endif /* IC_CVAR_H */
