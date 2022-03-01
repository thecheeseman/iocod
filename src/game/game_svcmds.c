#include "game_local.h"

void svcmd_entity_list(void)
{

}

void svcmd_add_ip(void)
{

}

void svcmd_remove_ip(void)
{

}

void svcmd_list_ip(void)
{

}

void svcmd_test_game(void)
{
	trap_printf("its a test from the game library\n");
}

void svcmd_say(void)
{
	if (g_dedicated.integer)
		trap_send_server_command(-1, 0, va("e \"GAME_SERVER\x15: %s\"",
										   concat_args(1)));
}

struct svcmd {
	const char *name;
	void (*func)(void);
};

static struct svcmd svcmds[] = {
	{ "entitylist", svcmd_entity_list },
	{ "addip", svcmd_add_ip },
	{ "removeip", svcmd_remove_ip },
	{ "listip", svcmd_list_ip },
	{ "testgame", svcmd_test_game },
	{ "say", svcmd_say },
	{ NULL }
};

bool console_command(void)
{
	char cmd[MAX_TOKEN_CHARS];
	struct svcmd *svcmd;

	trap_argv(0, cmd, sizeof(cmd));

	for (svcmd = svcmds; svcmd->name; svcmd++) {
		if (q_stricmp(cmd, svcmd->name) == 0) {
			svcmd->func();
			return true;
		}
	}

	if (g_dedicated.integer) {
		// everything else will also be printed as a say command
		trap_send_server_command(-1, 0, va("e \"GAME_SERVER\x15: %s\"",
										concat_args(0)));
		return true;
	}

	return false;
}