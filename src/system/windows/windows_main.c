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
 * @file windows_main.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"

//uid_t saved_euid;

void init_signals(void); // signals.c

void gpl_notice(void)
{
	fprintf(stderr, 
			"iocod, copyright(C) 2021 - 2022 thecheeseman\n" \
			"iocod comes with ABSOLUTELY NO WARRANTY; " \
			"for details use the command 'gplinfo'.\n" \
			"This is free software, andyou are welcome to " \
			"redistribute it under certain conditions; use 'gplinfo' " \
			"for details.\n");
}

int main(int argc, char* argv[])
{
	int len, i;
	char* cmdline;
	/*
	// go back to real user for config loads ??
	saved_euid = geteuid();
	seteuid(geteuid());

	sys_set_default_cd_path(""); // no cd path

	sys_check_for_version(argc, argv);

	// merge the command line
	for (len = i = 1; i < argc; i++)
		len += strlen(argv[i]) + 1;
	cmdline = z_malloc(len);
	*cmdline = 0;

	for (i = 1; i < argc; i++) {
		if (i > 1)
			strcat(cmdline, " ");
		strcat(cmdline, argv[i]);
	}

	//
	gpl_notice();
	//

	sys_events_init();
	com_init(cmdline);
	z_free(cmdline); // no longer needed
	net_init();

	sys_console_input_init();

	fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) | FNDELAY);
	init_signals();

	while (1) {
		sys_configure_fpu();
		com_frame();
	}
	*/

	fprintf(stderr, "hello windows\n");
	return 0;
}
