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
 * @file   iocod.h
 * @author thecheeseman
 * @date   2022-04-26
 * 
 * Global header for iocod project. Contains useful macros for platform and
 * compiler identification, features, etc., custom string library, memory
 * allocation functions.
 * 
 * Automatically includes `<stdarg.h>`, `<stddef.h>`, `<stdint.h>`, and 
 * `<stdio.h>`.
 */

#ifndef IOCOD_H
#define IOCOD_H

/*
================================================================================
Platform/utilities
================================================================================
*/

#include "iocod/platform.h"
#include "iocod/platform_endianness.h"
#include "iocod/sal.h"
#include "iocod/utils.h"
#include "iocod/types.h" 

/*
================================================================================
Standard library
================================================================================
*/

//
// c++
//
IC_BEGIN_C_DECLS

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
================================================================================
iocod includes
================================================================================
*/

#include "iocod/assert.h"
#include "iocod/client.h"
#include "iocod/commands.h"
#include "iocod/common.h"
#include "iocod/conf.h"
#include "iocod/console.h"
#include "iocod/cvar.h"
#include "iocod/events.h"
#include "iocod/fs.h"
#include "iocod/log.h"
#include "iocod/memory.h"
#include "iocod/metrics.h"
#include "iocod/network.h"
#include "iocod/parse.h"
#include "iocod/server.h"
#include "iocod/shared.h"
#include "iocod/string.h"
#include "iocod/system.h"
#include "iocod/threads.h"

//
extern qbool core_active;
extern qbool core_quit;

IC_PUBLIC
void core_init(int argc, char *argv[]);

IC_PUBLIC
void core_run(void);

IC_PUBLIC
void core_shutdown(void);

//
// c++
//
IC_END_C_DECLS

#endif /* IOCOD_H */
