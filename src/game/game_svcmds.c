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

void svcmd_say(void)
{
	if (g_dedicated.integer)
		trap_send_server_command(-1, 0, va("e \"GAME_SERVER\x15: %s\"",
										   concat_args(1)));
}

// because server doesn't have this enabled
void svcmd_configstrings(void)
{
	int i, sel, start, max;
	char s[MAX_STRING_CHARS];

	if (trap_argc() > 1) {
		// select a specific configstring
		trap_argv(1, s, sizeof(s));
		sel = atoi(s);

		if (sel > MAX_CONFIG_STRINGS || sel < 0)
			return;

		trap_get_config_string(sel, s, sizeof(s));
		g_printf("%-4d %s\n", sel, s);
	} else {
		// see all
		g_printf("#### config string\n");
		g_printf("------------------\n");
		for (i = 0; i < MAX_CONFIG_STRINGS; i++) {
			trap_get_config_string(i, s, sizeof(s));
			if (!s[0])
				continue;

			switch (i) {
				case CS_STATUS_ICONS: g_printf("status icons:\n"); break;
				case CS_HEAD_ICONS:	g_printf("head icons:\n"); break;
				case CS_LOCATIONS: g_printf("locations:\n"); break;
				case CS_TAG_CONNECTS: g_printf("tag connects:\n"); break;
				case CS_CLIENT_VAR_KEYS: g_printf("client cvar keys/values:\n"); break;
				case CS_MODELS: g_printf("models:\n"); break;
				case CS_SOUNDS: g_printf("sounds:\n"); break;
				case CS_FX: g_printf("fx:\n"); break;
				case CS_FX_ON_TAG: g_printf("fx on tag:\n"); break;
				case CS_SHELLSHOCKS: g_printf("shellshocks:\n"); break;
				case CS_MENUS: g_printf("menus:\n"); break;
				case CS_HINT_STRINGS: g_printf("hint strings:\n"); break;
				case CS_LOCALIZED_STRINGS: g_printf("localized strings:\n"); break;
				case CS_SHADERS: g_printf("shaders:\n"); break;
			}

			if (i >= CS_CLIENT_VAR_KEYS && i < CS_CLIENT_VAR_VALUES) {
				g_printf("%-4d %-32s ", i, s);

				trap_get_config_string((i - CS_CLIENT_VAR_KEYS) +
									   CS_CLIENT_VAR_VALUES,
									   s, sizeof(s));
				g_printf("%s\n", s);
			} else if (i >= CS_CLIENT_VAR_VALUES && i < CS_MODELS) {
				continue; // skip
			} else {
				g_printf("%-4d %s\n", i, s);
			}
		}
	}
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
	{ "say", svcmd_say },

	{ "configstrings", svcmd_configstrings },
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