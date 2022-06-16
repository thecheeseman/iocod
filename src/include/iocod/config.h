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
