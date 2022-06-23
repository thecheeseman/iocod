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

#ifndef IOCOD_VERSION_H
#define IOCOD_VERSION_H

/**
 * @defgroup version_macros Version Macros
 * @brief Macros for dealing with version data.
 * @{
 */

/**
 * @def IC_VERSION_STRING
 * @brief Current version string. Used by CMake to populate all version
 * related macros.
 */
#define IC_VERSION_STRING "0.169.0-dev"

/**
 * @def IC_VERSION_ENCODE
 * @brief Encode separate version numbers into one large number.
 */
#define IC_VERSION_ENCODE(maj, min, rev) \
    (((maj) * 1000000) + ((min) * 1000) + (rev))

/**
 * @def IC_VERSION_DECODE_MAJOR
 * @brief Decode the major version from the full version value.
 */
#define IC_VERSION_DECODE_MAJOR(version)        ((version) / 1000000)

/**
 * @def IC_VERSION_DECODE_MINOR
 * @brief Decode the minor version from the full version value.
 */
#define IC_VERSION_DECODE_MINOR(version)        ((version % 1000000) / 1000)

/**
 * @def IC_VERSION_DECODE_REVISION
 * @brief Decode the revision number from the full version value.
 */
#define IC_VERSION_DECODE_REVISION(version)     ((version) % 1000)

/**
 * @def IC_VERSION_MAJOR
 * @brief Major version number. Automatically filled out by CMake.
 */
#ifndef IC_VERSION_MAJOR
#define IC_VERSION_MAJOR 0
#endif

/**
 * @def IC_VERSION_MINOR
 * @brief Minor version number. Automatically filled out by CMake.
 */
#ifndef IC_VERSION_MINOR
#define IC_VERSION_MINOR 0
#endif

/**
 * @def IC_VERSION_PATCH
 * @brief Patch version number. Automatically filled out by CMake.
 */
#ifndef IC_VERSION_PATCH
#define IC_VERSION_PATCH 0
#endif

/**
 * @def IC_VERSION
 * @brief The numeric representation of the version.
 */
#define IC_VERSION \
    IC_VERSION_ENCODE(IC_VERSION_MAJOR, IC_VERSION_MINOR, IC_VERSION_PATCH)

/** @} */

#endif /* IOCOD_VERSION_H */
