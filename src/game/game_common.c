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

#include "game_local.h"
#include "common/error.h"
#include "common/print.h"
#include "stringlib.h"

// replacement for com_printf
void g_printf(const char *fmt, ...)
{
	va_list argptr;
	char msg[1024];

	va_start(argptr, fmt);
	vsnprintf(msg, sizeof(msg), fmt, argptr);
	va_end(argptr);

	trap_printf(msg);
}

// replacement for com_dprintf
void g_dprintf(const char *fmt, ...)
{
	va_list argptr;
	char msg[1024];

	va_start(argptr, fmt);
	vsnprintf(msg, sizeof(msg), fmt, argptr);
	va_end(argptr);

	if (g_developer.integer > 0)
		trap_printf(msg);
}

// replacement for com_error
void g_error(const char *fmt, ...)
{
	va_list argptr;
	char err[1024];

	va_start(argptr, fmt);
	vsnprintf(err, sizeof(err), fmt, argptr);
	va_end(argptr);

	trap_error(err);
}

void com_printf_runner(enum print_level level, const char *fmt, ...)
{
	va_list argptr;
	char msg[1024];

	UNUSED(level);

	va_start(argptr, fmt);
	vsnprintf(msg, sizeof(msg), fmt, argptr);
	va_end(argptr);

	g_printf("%s", msg);
}

void com_error_runner(enum error_code code, const char *file, const char *func,
					  int line, const char *fmt, ...)
{
	va_list argptr;
	char err[1024];

	UNUSED(code);
	UNUSED(file);
	UNUSED(func);
	UNUSED(line);

	va_start(argptr, fmt);
	vsnprintf(err, sizeof(err), fmt, argptr);
	va_end(argptr);

	g_error("%s", err);
}

void g_log_printf(const char *fmt, ...)
{
	va_list	argptr;
	char string[MAX_PRINT_MSG];
	int	min, tens, sec;

	sec = level.time / 1000;

	min = sec / 60;
	sec -= min * 60;
	tens = sec / 10;
	sec -= tens * 10;

	com_sprintf(string, sizeof(string), "%3i:%i%i ", min, tens, sec);

	va_start(argptr, fmt);
	vsprintf(string + 7, fmt, argptr);
	va_end(argptr);

	if (g_dedicated.integer > 0)
		g_printf("%s", string + 7);

	if (level.logfile == 0)
		return;

	trap_fs_write(string, strlen(string), level.logfile);
}
