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

#ifndef IOCOD_FS_H
#define IOCOD_FS_H

#include "iocod/platform.h"
#include "iocod/types.h"

#define BASEGAME "main"
#define DEFAULTCFG "default_mp.cfg"

#define MAX_OSPATH 256

IC_PUBLIC
void fs_init(void);

IC_PUBLIC
void fs_shutdown(qbool a);

#endif /* IOCOD_FS_H */
