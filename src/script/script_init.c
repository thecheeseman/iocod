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
 * @file script_init.c
 * @date 2022-02-04
*/

#include "script_local.h"

int scr_developer;			// local copy of cvar "developer"
int scr_developer_script;	// local copy of cvar "developer_script"
int scr_developer_mode;		// 1 or 2 depending on the above values
int scr_developer_log;		// added by 1.5

bool scr_system_active;
bool scr_loading_scripts;
bool scr_initialized;

char *scr_error_message;
bool scr_error_entered;

int scr_stack_params;
uint16_t *scr_stack_pointer;
#if 0
void scr_init(int developer, int developer_script)
{
	scr_developer = developer;
	scr_developer_script = developer_script;

	/*
	fun_080a22d8();
	_DAT_082f57e0 = &DAT_082f57d8;
	scr_stack = &DAT_082f17e0;
	DAT_082f57e4 = 0;
	DAT_082f5880 = 0;
	DAT_082f5884 = 0;
	DAT_082f5888 = 0;
	DAT_082f588c = 0;
	scr_stack_params = 0;
	DAT_082f5940 = 0;
	uVar1 = FUN_080a24f8();
	*/
	scr_system_active = false;

	// DAT_080e31ce = (short) uVar1;
	// DAT_080e31d0 = 1;

	scr_loading_scripts = false;
	scr_initialized = true;

	/*
	DAT_081613b9 = 0;
	DAT_081613ba = 0;
	DAT_081613bc = 0;
	DAT_081613be = 0;
	DAT_082f17e4 = 6;
	_DAT_082f5890 = 0;
	DAT_082f5950 = 0;
	DAT_082f5952 = 0;
	DAT_082f5954 = 0;
	DAT_082f5956 = 0;
	*/
}

void scr_abort(void)
{
	scr_system_active = false;
	scr_initialized = false;
}

void scr_shutdown(void)
{
	if (scr_initialized)
		scr_initialized = false;
}
#endif
