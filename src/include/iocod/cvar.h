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
 * @defgroup ic_cvar Cvars
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
 * RTCW/COD have this set to 1024/2048 respectively.
 */
#define MAX_CVARS 4096

/**
 * @enum cv_flags
 * @brief Cvar flags.
*/
enum cv_flags {
    /**
     * @brief No cvar flags.
    */
    CV_NONE         = 0,

    /**
     * @brief Variables which are automatically written to the config files.
    */
    CV_ARCHIVE      = 1,

    /**
     * @brief Sent to the server on client connect or change.
    */
    CV_USER_INFO    = 2,

    /**
     * @brief Sent in response to frontend requests.
    */
    CV_SERVER_INFO  = 4,

    /**
     * @brief Cvars which are duplicated on all clients.
    */
    CV_SYSTEM_INFO  = 8, 

    /**
     * @brief Only allow changes from the command line (no console changes).
    */
    CV_INIT         = 16,

    /**
     * @brief Latched until the next @ref cv_get() call.
    */
    CV_LATCH        = 32,

    /**
     * @brief Display only, cannot be set by the user at all.
    */
    CV_ROM          = 64,

    /**
     * @brief Variables created by the `set` family of commands.
    */
    CV_USER_CREATED = 128,

    /**
     * @brief Can be set even when cheats are disabled, but is not archived.
    */
    CV_TEMP         = 256,

    /**
     * @brief Cannot be changed if cheats are disabled.
    */
    CV_CHEAT        = 512,

    /**
     * @brief Do not clear when `cvar_restart` is issued.
    */
    CV_NO_RESTART   = 1024,

    /**
     * @brief Added in RTCW. Could be CV_SERVER_CREATED
    */
    CV_WOLF_INFO    = 2048,

    /**
     * @brief Unknown. Could be CV_PROTECTED
    */
    CV_4096         = 4096,

    /**
     * @brief Unknown. Added in COD1.
    */
    CV_8192         = 8192
};

/**
 * @brief Cvar floating point type.
 * @note All Q3-based games use `float`.
*/
typedef float cv_float;

/**
 * @brief Cvar integer type.
 * @note All Q3-based games use `int`.
*/
typedef int cv_int;

/**
 * @brief Cvar structure.
 * 
 * @warning Nothing outside of the `cv_*()` functions should modify
 * these fields!
*/
struct cvar {
    /**
     * @brief Name of cvar.
    */
    char *name;
    
    /**
     * @brief Cvar string value.
    */
    char *string;

    /**
     * @brief Issuing a `cvar_restart` will reset the cvar to this value.
    */
    char *reset_string;

    /**
     * @brief Used for #CV_LATCH flagged cvars.
    */
    char *latched_string;
    
    /**
     * @brief Cvar flags.
    */
    enum cv_flags flags;
    
    /**
     * @brief Set each time the cvar is modified.
    */
    bool modified;

    /**
     * @brief Number of total times this cvar has been changed.
    */
    int modification_count;

    /**
     * @brief Floating-point representation of the value.
    */
    cv_float value;

    /**
     * @brief Integer representation of the value.
    */
    cv_int integer;

    /**
     * @brief Next cvar in the linked list.
    */
    struct cvar *next;

    /**
     * @brief Next hashtable entry.
    */
    struct cvar *hash_next;
};

/**
 * @def MAX_VMCVAR_STRING_LEN
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

/*
 * 
 * cv_commands.c
 *
 */

/*
 *
 * cv_get.c 
 * 
 */

/**
 * @brief Find a cvar by name.
 * 
 * @param[in] name name of the cvar to look for
 * 
 * @return NULL if not found, otherwise a pointer to a @ref cvar structure
*/
IC_PUBLIC
struct cvar *cv_find(const char *name);

IC_PUBLIC
/**
 * @brief Get or set a cvar.
 * 
 * Despite it's name, this function does two things. First, it checks if the
 * given cvar exists, and if not it will create it automatically with the
 * default @p value and @p flags as specified. 
 
 * If it already exists, and it's flagged as @ref CV_USER_CREATED, it will 
 * set the @ref cvar.reset_string to the value passed and update the flags. 
 * If there is already a string latched, it will immediately take the 
 * latched value.
 * 
 * @param[in] name  cvar name
 * @param[in] value default value
 * @param[in] flags cvar flags
 * 
 * @return NULL if failed, otherwise a pointer to a @ref cvar structure
*/
struct cvar *cv_get(const char *name, const char *value, enum cv_flags flags);

/*
 *
 * cv_infostring.c 
 *
 */

/**
 * @brief Return all cvars identified by given mask as a regular infostring.
 * 
 * @param[in] mask cvar flags mask
 * 
 * @return infostring containing all matched cvars.
*/
IC_PUBLIC
char *cv_infostring(enum cv_flags mask);

/**
 * @brief Return all cvars identified by given mask as a big infostring.
 * 
 * @param[in] mask cvar flags mask
 * 
 * @return infostring containing all matched cvars.
*/
IC_PUBLIC
char *cv_big_infostring(enum cv_flags mask);

/**
 * @brief Copy all cvars identified by given mask into a char[] buffer.
 * 
 * @param[in] mask cvar flags mask
 * @param[out] buf string buffer
 * @param[in] size size of string buffer
*/
IC_PUBLIC
IC_NON_NULL(2)
void cv_infostring_buffer(enum cv_flags mask, char *buf, size_t size);

/*
 *
 * cv_init.c
 *
 */

/**
 * @brief Initialise cvar system.
 */
IC_PUBLIC
void cv_init(void);

/**
 * @brief Shutdown cvar system.
 */
IC_PUBLIC
void cv_shutdown(void);

/*
 *
 * cv_set.c 
 *
 */

/**
 * @brief Try to set a cvar with the given string value.
 * 
 * If the given cvar does not already exist and @p force is `false`, this will
 * create a new cvar with a @ref CV_USER_CREATED flag. If @p force is `true`, 
 * this will create it with no flags at all.
 * 
 * If the cvar already exists, and @p force is `false`, will set its 
 * @ref cvar.latched_string value. If @p force is `true`, then will always 
 * try to set the value (unless it is blocked by a @ref CV_ROM, @ref CV_LATCH, 
 * @ref CV_INIT, or @ref CV_CHEAT flag).
 * 
 * @param[in] name  name of the cvar
 * @param[in] value value to set the cvar to
 * @param[in] force whether to force this cvar to be set
 * 
 * @return NULL if failed, or pointer to a @ref cvar structure otherwise
*/
IC_PUBLIC
struct cvar *cv_set2(const char *name, const char *value, bool force);

/**
 * @brief Set a cvar with the given string value, always forcing the value
 * to be set if possible.
 *
 * @param[in] name  name of the cvar
 * @param[in] value value to set the cvar to
 * 
 * @see cv_set2
 *
 * @return NULL if failed, or pointer to a @ref cvar structure otherwise
*/
IC_PUBLIC
struct cvar *cv_set_string(const char *name, const char *value);

/**
 * @brief Set a cvar's @ref cvar.latched_string with the given value.
 *
 * @param[in] name  name of the cvar
 * @param[in] value value to set the cvar to
 *
 * @see cv_set2
 *
 * @return NULL if failed, or pointer to a @ref cvar structure otherwise
*/
IC_PUBLIC
struct cvar *cv_set_string_latched(const char *name, const char *value);


/**
 * @brief Set a cvar with the given floating point value.
 *
 * @param[in] name  name of the cvar
 * @param[in] value value to set the cvar to
 *
 * @see cv_set2
 *
 * @return NULL if failed, or pointer to a @ref cvar structure otherwise
*/
IC_PUBLIC
struct cvar *cv_set_value(const char *name, cv_float value);

/**
 * @brief Set a cvar with the given integer value.
 *
 * @param[in] name  name of the cvar
 * @param[in] value value to set the cvar to
 *
 * @see cv_set2
 *
 * @return NULL if failed, or pointer to a @ref cvar structure otherwise
*/
IC_PUBLIC
struct cvar *cv_set_integer(const char *name, cv_int value);

/**
 * @brief Reset a cvar to its @ref cvar.reset_string value.
 *
 * @param[in] name  name of the cvar
 *
 * @return NULL if failed, or pointer to a @ref cvar structure otherwise
*/
IC_PUBLIC
struct cvar *cv_reset(const char *name);

/*
 *
 * cv_string.c 
 *
 */

/**
 * @brief Remove any extended ASCII characters from the given string.
 * 
 * @param[in] value string to clean
 * 
 * @return a cleaned string
 */
IC_PUBLIC
char *cv_clear_foreign_chars(const char *value);

/**
 * @brief Return a cvar's string value.
 * 
 * @param[in] name name of the cvar to search for
 * 
 * @return a string containing the value
*/
IC_PUBLIC
IC_NON_NULL(1)
char *cv_get_string(const char *name);

/**
 * @brief Return a cvar's string value into the given string buffer.
 * 
 * @param[in]  name  name of the cvar to search for
 * @param[out] buf   pointer to string buffer
 * @param[in]  size  size of the string buffer
*/
IC_PUBLIC
IC_NON_NULL(1, 2)
void cv_get_string_buffer(const char *name, char *buf, size_t size);

/**
 * @brief Validate a cvar string.
 * 
 * @param[in] s string to validate
 * 
 * @return false if string contains '\' or '"' or ';' chars, true otherwise
*/
IC_PUBLIC
bool cv_validate_string(const char *s);

/*
 *
 * cv_value.c 
 *
 */

/**
 * @brief Return a cvar's floating point value.
 * 
 * @param[in] name name of the cvar to search for
 * 
 * @return a float containing the value
*/
IC_PUBLIC
IC_NON_NULL(1)
cv_float cv_get_value(const char *name);

/**
 * @brief Return a cvar's integer value.
 * 
 * @param[in] name name of the cvar to search for
 * 
 * @return an integer containing the value
*/
IC_PUBLIC
IC_NON_NULL(1)
cv_int cv_get_integer(const char *name);

/*
 *
 * cv_write.c 
 *
 */

/**
 * @brief Write cvar defaults to a given file. 
 * 
 * This will actively ignore any cvar that has a @ref CV_ROM, 
 * @ref CV_USER_CREATED, @ref CV_CHEAT, or @ref CV_4096 flag set.
 *
 * @param f handle to output file
 * 
 * @return true if successful, false otherwise
 */
IC_PUBLIC
bool cv_write_defaults(filehandle f);

/**
 * @brief Write archived cvar variables to a given file. Only cvars with the
 * @ref CV_ARCHIVE flag are written.
 *
 * @param f handle to output file
 *
 * @return true if successful, false otherwise
 */
IC_PUBLIC
bool cv_write_variables(filehandle f);

/** @} */

#endif /* IC_CVAR_H */
