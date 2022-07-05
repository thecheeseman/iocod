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

#ifndef IC_CONFIG_H
#define IC_CONFIG_H

#include "iocod/platform.h"
#include "iocod/types.h"

/**
 * @defgroup conf Config
 * @brief Config module for handling configuration files. Can be used by
 * any internal system, or the module system.
 * 
 * Example usage:
 * @code
 * struct conf *cfg;
 * struct confopt opts[] = {
 *      // options go here
 *      CONF_HEADER("Section header"),
 *      CONF_COMMENT("This variable will control whether something happens"),
 *      CONF_BOOL("variable", off),
 * 
 *      CONF_HEADER("Another section"),
 *      CONF_INT("some_value", 42),
 *      CONF_STRING("name", "billy bob"),
 * 
 *      // Structure must _always_ end with this
 *      CONF_END()
 * };
 * 
 * void conf_example(void)
 * {
 *      cfg = conf_init("test.conf", opts, 0);
 *      
 *      // do something with the config
 *      if (!conf_get_bool("variable"))
 *          // ...
 * 
 *      // shutdown
 *      conf_shutdown(cfg);
 * }
 * @endcode
 * @{
 */

typedef int conf_int;
typedef float conf_float;

/**
 * @brief Config key/value pair types.
*/
enum confopt_type {
    /**
     * @brief Blank (default) key/value type. Does nothing.
    */
    CONF_BLANK,

    /**
     * @brief Basic comment line in config. Can be prefixed with '#', '//' or
     * ';', depending on the config comment style setting.
    */
    CONF_COMMENT,

    /**
     * @brief Headers. Try to keep <76 chars.
     *
     * @code
     * // config entry 
     * CONF_HEADER("This is a header"),
     * 
     * // will output this into the conf file:
     * ####################
     * # This is a header #
     * ####################
     * @endcode
    */
    CONF_HEADER,

     /**
     * @brief Sections.
     *
     * @code
     * // config entry
     * CONF_SECTION("This is a section"),
     *
     * // will output this into the conf file:
     * #
     * # This is a section
     * #
     * @endcode
    */
    CONF_SECTION,

    /**
     * @brief Boolean value.
     * 
     * Valid options for booleans are `yes`, `no`, `on`, `off`, `true`, 
     * and `false`.
    */
    CONF_BOOL,

    /**
     * @brief Integer value.
    */
    CONF_INT,

    /**
     * @brief Floating point value.
    */
    CONF_FLOAT,

    /**
     * @brief String value.
    */
    CONF_STRING,

    /**
     * @brief Used to mark the end of a config structure. Every config 
     * must end with this.
     * 
     * Example:
     * @code
     * struct confopt opts[] = {
     *      CONF_HEADER("Some header"),
     *      CONF_BOOL("setting", yes),
     *      CONF_INT("another_setting", 10)
     *      CONF_END()
     * };
     * @endcode
    */
    CONF_END
};

/**
 * @brief Config key/value pair options.
*/
struct confopt {
    /**
     * @brief Name of option.
    */
    const char *name;

    /**
     * @brief Option type.
    */
    enum confopt_type type;

    /**
     * @brief Default string.
    */
    char *default_str;

    /**
     * @brief Current value, determined by @ref confopt.type
    */
    union value {
        conf_int i;
        conf_float f;
        char *s;
    } value;
};

/**
 * @def CONF_HEADER
 * @brief Insert header @p x into the config structure.
 */
#define CONF_HEADER(x) { .name = "hdr", .type = CONF_HEADER, .default_str = x }

/**
 * @def CONF_SECTION
 * @brief Insert section @p x into the config structure.
 */
#define CONF_SECTION(x) { .name = "sec", .type = CONF_SECTION, .default_str = x }

/**
 * @def CONF_COMMENT
 * @brief Insert comment @p x into the config structure.
 */
#define CONF_COMMENT(x) { .name = "cmt", .type = CONF_COMMENT, .default_str = x }

/**
 * @def CONF_BLANK
 * @brief Insert a blank line into the config structure.
 */
#define CONF_BLANK() { .name = "blk", .type = CONF_BLANK }

/**
 * @def CONF_BOOL
 * @brief Insert boolean @p n with default value @p v into the config structure.
 * 
 * Valid options for booleans are `yes`, `no`, `on`, `off`, `true`, and `false`.
 * Automatically converts boolean values to strings (required internally), so 
 * correct usage looks like this:
 * 
 * @code
 * // ...
 * CONF_BOOL("some_setting", true),
 * CONF_BOOL("other_setting", yes),
 * // etc ...
 * @endcode
 */
#define CONF_BOOL(n, v) { .name = n, .type = CONF_BOOL, .default_str = #v }

/**
 * @def CONF_INT
 * @brief Insert integer @p n with default value @p v into the config structure.
 */
#define CONF_INT(n, v) { .name = n, .type = CONF_INT, .default_str = #v }

/**
 * @def CONF_FLOAT
 * @brief Insert float @p n with default value @p v into the config structure.
 */
#define CONF_FLOAT(n, v) { .name = n, .type = CONF_FLOAT, .default_str = #v }

/**
 * @def CONF_STRING
 * @brief Insert string @p n with default value @p v into the config structure.
 */
#define CONF_STRING(n, v) { .name = n, .type = CONF_STRING, .default_str = v }

/**
 * @def CONF_END
 * @brief End of config structure. 
 * @warning This is a required element and must be the last element of your
 * config structure array.
 * 
 * Example:
 * @code
 * struct confopt opts[] = {
 *      CONF_HEADER("Some header"),
 *      CONF_BOOL("setting", yes),
 *      CONF_INT("another_setting", 10)
 *      CONF_END()
 * };
 * @endcode
 */
#define CONF_END()       { .name = "", .type = CONF_END }

#define MAX_CONFIG_TOKEN 1024

/**
 * @brief Config.
*/
struct conf {
    /**
     * @brief Filename.
    */
    char *filename;

    /**
     * @brief Size of file.
    */
    size_t size;

    /**
     * @brief Config key/value pairs.
    */
    struct confopt *options;

    // internal stuff that doesn't need documenting
    char *buffer;
    char *script_p;
    char *end_p;
    int line;
    int script_line;
    bool end;
    char token[MAX_CONFIG_TOKEN];
    // internal stuff that doesn't need documenting
};

/**
 * @brief Initialize a config file.
 * 
 * @param[in] filename name of config file
 * @param[in] kv       pointer to config key/value pairs
 * @param[in] options  any options to set for the config
 * @return NULL on failure, otherwise pointer to new config
 */
IC_PUBLIC
struct conf *conf_init(const char *filename, struct confopt *kv, int options);

/**
 * @brief Shutdown config file. Automatically frees any dynamically allocated
 * memory associated with config.
 * 
 * @param[in] cfg config file to shutdown
 * @return true if success, false if failure
 */
IC_PUBLIC
bool conf_shutdown(struct conf *cfg);

/**
 * @brief Dump all loaded config opts/values to stdout.
 * 
 * @param[in] cfg config file to dump
 */
IC_PUBLIC
void conf_dump_options(struct conf *cfg);

/**
 * @brief Get a config key/value pair.
 * 
 * @param[in] cfg  config to check
 * @param[in] name name of key/value pair
 * @return NULL if not found, pointer to key/value pair otherwise
 */
IC_PUBLIC
struct confopt *conf_get_opt(struct conf *cfg, const char *name);

/**
 * @brief Get a config boolean.
 * 
 * @param[in] cfg  config to check
 * @param[in] name name of key to get
 * @return value of key (if exists), otherwise default value for type
 */
IC_PUBLIC
bool conf_get_bool(struct conf *cfg, const char *name);

/**
 * @brief Get a config integer.
 *
 * @param[in] cfg  config to check
 * @param[in] name name of key to get
 * @return value of key (if exists), otherwise default value for type
 */
IC_PUBLIC
conf_int conf_get_int(struct conf *cfg, const char *name);

/**
 * @brief Get a config float.
 *
 * @param[in] cfg  config to check
 * @param[in] name name of key to get
 * @return value of key (if exists), otherwise default value for type
 */
IC_PUBLIC
conf_float conf_get_float(struct conf *cfg, const char *name);

/**
 * @brief Get a config string.
 *
 * @param[in] cfg  config to check
 * @param[in] name name of key to get
 * @return value of key (if exists), otherwise default value for type
 */
IC_PUBLIC
char *conf_get_string(struct conf *cfg, const char *name);

/**
 * @brief Set a config boolean.
 *
 * @param[in] cfg   config to check
 * @param[in] name  name of key to get
 * @param[in] value value to set
 * @return true if success, false otherwise
 */
IC_PUBLIC
bool conf_set_bool(struct conf *cfg, const char *name, bool value);

/**
 * @brief Set a config integer.
 *
 * @param[in] cfg   config to check
 * @param[in] name  name of key to get
 * @param[in] value value to set
 * @return true if success, false otherwise
 */
IC_PUBLIC
bool conf_set_int(struct conf *cfg, const char *name, conf_int value);

/**
 * @brief Set a config float.
 *
 * @param[in] cfg   config to check
 * @param[in] name  name of key to get
 * @param[in] value value to set
 * @return true if success, false otherwise
 */
IC_PUBLIC
bool conf_set_float(struct conf *cfg, const char *name, conf_float value);

/**
 * @brief Set a config string. Will automatically deallocate old string
 * and allocate new one.
 *
 * @param[in] cfg   config to check
 * @param[in] name  name of key to get
 * @param[in] value value to set
 * @return true if success, false otherwise
 */
IC_PUBLIC
bool conf_set_string(struct conf *cfg, const char *name, char *value);

/**
 * @brief Initialize iocod config.
 */
IC_PUBLIC
void config_init(void);

/**
 * @brief Shutdown iocod config.
 */
IC_PUBLIC
void config_shutdown(void);

/**
 * @brief Return a reference to the iocod config.
 */
IC_PUBLIC
struct conf *config_get(void);

/**
 * @brief Get the log level set in the iocod config.
 * @return Returns the log level. If the log_level option can't be found, or is 
 * out of range, will always return @ref LOG_LEVEL_INFO
 */
IC_PUBLIC
int config_log_level(void);

/**
 * @brief Get the preferred console language from the iocod config.
 * @return The language specified, otherwise on error just returns "english".
*/
IC_PUBLIC
char *config_console_language(void);

/**
 * @brief Check if the iocod config is initialized.
 * @return true if it is, false otherwise
 */
IC_PUBLIC
bool config_initialized(void);

/** @} */

#endif /* IC_CONFIG_H */
