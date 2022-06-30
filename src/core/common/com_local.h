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

#ifndef COM_LOCAL_H
#define COM_LOCAL_H

#include "iocod.h"
#include <setjmp.h>

extern jmp_buf abortframe;
extern bool error_entered;
extern int com_frame_time;

/**
 * @brief Add common commands to the command system.
*/
void com_add_commands(void);

void parse_command_line(char *cmdline);
void startup_variable(const char *match);
bool add_startup_commands(void);

/**
 * @brief Set up random seed with a system-defined seed, or time(NULL) if 
 * system could not provide a seed.
*/
void rand_init(void);

#endif /* COM_LOCAL_H */
