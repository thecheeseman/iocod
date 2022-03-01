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
 * @file cvar_local.h
 * @date 2022-02-04
*/

#ifndef __CVAR_LOCAL_H__
#define __CVAR_LOCAL_H__

#include "shared.h"
#include "common.h"

extern struct cvar *cvar_vars;
extern struct cvar *cvar_cheats;

#define MAX_CVARS 1024
extern struct cvar cvar_indexes[MAX_CVARS];
extern int cvar_num_indexes;

#define FILE_HASH_SIZE 256
extern struct cvar *hashtable[FILE_HASH_SIZE];

struct cvar *cvar_find_var(const char *var_name);

#endif // __CVAR_LOCAL_H__
