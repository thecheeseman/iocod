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
 * @file files_cmds.c
 * @date 2022-02-04
*/

#include "files_local.h"

void fs_path(bool ignorelocalized)
{
	struct searchpath *sp;
	int i;

	if (fs_ignorelocalized->integer > 0)
		com_printf("    localized assets are being ignored\n");

	com_printf("Current search path:\n");

	for (sp = fs_searchpaths; sp != NULL; sp = sp->next) {
		if (!ignorelocalized || fs_search_localized(sp)) {
			if (sp->pack != NULL) {
				com_printf("%s (%d files)\n", sp->pack->pak_filename,
						   sp->pack->numfiles);

				if (fs_num_serverpaks > 0) {
					if (fs_pak_is_pure(sp->pack))
						com_printf("    not on the pure list\n");
					else
						com_printf("    on the pure list\n");
				}
			} else {
				com_printf("%s/%s\n", sp->dir->path, sp->dir->gamedir);
			}
		}
	}

	com_printf("\nFile Handles:\n");
	for (i = 1; i < MAX_FILE_HANDLES; i++) {
		if (fsh[i].handlefiles.file.o != NULL)
			com_printf("handle %d: %s\n", i, fsh[i].name);
	}
}

static void fs_path_f(void)
{
	fs_path(false);
}

void fs_add_commands(void)
{
	cmd_add_command("path", fs_path_f);
}

void fs_remove_commands(void)
{
	cmd_remove_command("path");
}
