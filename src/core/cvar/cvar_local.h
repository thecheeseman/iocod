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

#ifndef CVAR_LOCAL_H
#define CVAR_LOCAL_H

#include "iocod.h"

#define FILE_HASH_SIZE 256

extern struct cvar *cvars;
extern struct cvar indexes[MAX_CVARS];
extern size_t num_indexes;
extern struct cvar *hashtable[FILE_HASH_SIZE];
extern struct cvar *sv_console_lockout;
extern struct cvar *com_sv_running;
extern struct cvar *cv_cheats;

void cv_list_f(void);
void cv_print_f(void);
void cv_set_f(void);
void cv_toggle_f(void);
void cv_unset_f(void);

#endif /* CVAR_LOCAL_H */
