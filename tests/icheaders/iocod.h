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

#ifndef IOCOD_H
#define IOCOD_H

/*
 * Always include config.h  
 */
#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// temporary
#define ic_warn_if(expr, warning)
#define ic_warning(warning)
#define ic_error(err)
#define ic_error_fatal(err)

/*
 * General useful macros / utils
 */
#include "ic_utils.h"

/*
 * Version utilities from hedley https://github.com/nemequ/hedley
 */
#define IC_VERSION_ENCODE(maj, min, rev) \
    (((maj) * 1000000) + ((min) * 1000) + (rev))
#define IC_VERSION_DECODE_MAJOR(version)        ((version) / 1000000)
#define IC_VERSION_DECODE_MINOR(version)        ((version % 1000000) / 1000)
#define IC_VERSION_DECODE_REVISION(version)     ((version) % 1000)

#define IC_VERSION \
    IC_VERSION_ENCODE(IC_VERSION_MAJOR, IC_VERSION_MINOR, IC_VERSION_PATCH)
#define IC_VERSION_STRING "v" \
    IC_STRINGIFY(IC_VERSION_MAJOR) "." \
    IC_STRINGIFY(IC_VERSION_MINOR) "." \
    IC_STRINGIFY(IC_VERSION_PATCH)

/*
 * Platform information
 */
#include "ic_platform.h"

/*
 * Assert utilities
 */
#include "ic_assert.h"

/*
 * Memory
 */
#include "ic_memory.h"

/*
 * Common types
 */
#include "ic_types.h"

/*
 * SAL-like qualifiers  
 */
#include "ic_sal.h"

/*
 * Strings
 */
#include "ic_string.h"

#endif /* IOCOD_H */
