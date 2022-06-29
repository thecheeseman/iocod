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

#ifndef COMMAND_LOCAL_H
#define COMMAND_LOCAL_H

#include "iocod.h"

/**
 * @defgroup command_local Commands (local)
 * @brief Local command procedures and structures needed for command module.
 * @{
 */

#define	MAX_CMD_BUFFER  128*1024
#define	MAX_CMD_LINE	1024

// args.c
extern unsigned int argc;
extern char *argv[MAX_STRING_TOKENS];

// tokenize_string.c
extern char cmd_tokenized[INFO_STRING_BIG + MAX_STRING_TOKENS];
extern char cmd_cmd[INFO_STRING_BIG];

extern struct cmd *cmd_functions;
extern size_t cmd_wait;

/** @} */

#endif /* COMMAND_LOCAL_H */
