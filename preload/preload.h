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
 * @file preload.h
 * @date 2022-02-19
 *
 * This file is for the creation of the LD_PRELOAD hook (based on CoDExtended)
 * which we can use to test the game dll against the original code. It also
 * enables stacktracing and a number of other useful features for debugging
 * the original code.
*/

#ifndef __PRELOAD_H__
#define __PRELOAD_H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <time.h>
#include <termios.h>
#include <sys/signal.h>
#include <string.h>
#include <execinfo.h>
#include <err.h>
#include <stdint.h>
#include <sys/types.h>

// backtrace options
enum backtrace_options {
	BT_NONE = 0x00, /**< no signals */
	BT_INT = 0x01,	/**< terminal interrupt */
	BT_ILL = 0x02,	/**< illegal instruction */
	BT_ABRT = 0x04, /**< abort */
	BT_FPE = 0x08,	/**< erroneous arithmetic operation */
	BT_SEGV = 0x10, /**< segmentation fault */
	BT_TERM = 0x20, /**< terminate */
	BT_ALL = 0x7F	/**< catch all signals */
};

#define BT_STD  (BT_ILL | BT_ABRT | BT_FPE | BT_FPE | BT_SEGV | BT_TERM)
//

void backtrace_init(enum backtrace_options options);
void preload_lib_exit(void);

typedef void (*cmd_t)(void);

#define DECLARATIONS_INCLUDE
#include "declarations.h"

DECLARE_2(_cmd_add_command, int, const char *, cmd_t)
DECLARE_1(_com_quit, void, void)
DECLARE_1(_sys_console_input_shutdown, void, void)

#define DECLARATIONS_CLEANUP
#include "declarations.h"

//
void __call(int, int);
//

#endif // __PRELOAD_H__
