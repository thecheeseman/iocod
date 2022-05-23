#ifndef IC_CONFIG_H
#define IC_CONFIG_H

#include "iocod.h"

/**
 * @defgroup config Config
 * @brief Config handling
 * @{
 */

/**
 * @brief Get config boolean value. Wrapper for `cfg_getbool()`.
 * @param[in] name    name of config value to get
 * @return boolean value
*/
IC_PUBLIC
bool config_get_bool(const char *name);

/**
 * @brief Get config integer value. Wrapper for `cfg_getint()`.
 * @param[in] name    name of config value to get
 * @return integer value
*/
IC_PUBLIC
long config_get_int(const char *name);

/**
 * @brief Get config floating point value. Wrapper for `cfg_getint()`.
 * @param[in] name    name of config value to get
 * @return double value
*/
IC_PUBLIC
double config_get_float(const char *name);

/**
 * @brief Get config string value. Wrapper for `cfg_getstr()`.
 * @param[in] name    name of config value to get
 * @return string value
*/
IC_PUBLIC
char *config_get_string(const char *name);

IC_PUBLIC
void config_set_bool(const char *name, bool value);

IC_PUBLIC
void config_set_int(const char *name, long value);

IC_PUBLIC
void config_set_float(const char *name, double value);

IC_PUBLIC
void config_set_string(const char *name, const char *value);

IC_PUBLIC
void config_init(void);

IC_PUBLIC
void config_shutdown(void);

/** @} */

#endif /* IC_CONFIG_H */
