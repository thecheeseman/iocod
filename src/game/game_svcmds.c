#include "game_local.h"

struct ip_filter {
	unsigned int mask;
	unsigned int compare;
};

#define MAX_IP_FILTERS 1024

static struct ip_filter ip_filters[MAX_IP_FILTERS];
static int num_ip_filters;

static bool string_to_filter(char *s, struct ip_filter *f) 
{
	char num[128];
	int i, j;
	byte b[4];
	byte m[4];

	for (i = 0; i < 4; i++) {
		b[i] = 0;
		m[i] = 0;
	}

	for (i = 0; i < 4; i++) {
		if (*s < '0' || *s > '9') {
			if (*s == '*') { // 'match any'
				// b[i] and m[i] to 0
				s++;
				if (!*s)
					break;

				s++;
				continue;
			}

			g_printf("Bad filter address: %s\n", s);
			return false;
		}

		j = 0;
		while (*s >= '0' && *s <= '9')
			num[j++] = *s++;

		num[j] = 0;
		b[i] = atoi(num);
		m[i] = 255;

		if (!*s)
			break;

		s++;
	}

	f->mask = *(unsigned int *) m;
	f->compare = *(unsigned int *) b;

	return true;
}

static void update_ip_bans(void) 
{
	byte b[4];
	byte m[4];
	int i, j;
	char iplist_final[MAX_CVAR_VALUE_STRING];
	char ip[64];

	*iplist_final = 0;
	for (i = 0; i < num_ip_filters; i++) {
		if (ip_filters[i].compare == 0xffffffff)
			continue;

		*(unsigned int *) b = ip_filters[i].compare;
		*(unsigned int *) m = ip_filters[i].mask;
		*ip = 0;
		for (j = 0; j < 4; j++) {
			if (m[j] != 255) 
				q_strcat(ip, sizeof(ip), "*");
			else
				q_strcat(ip, sizeof(ip), va("%i", b[j]));

			q_strcat(ip, sizeof(ip), (j < 3) ? "." : " ");
		}

		if (strlen(iplist_final) + strlen(ip) < MAX_CVAR_VALUE_STRING) {
			q_strcat(iplist_final, sizeof(iplist_final), ip);
		} else {
			g_printf("g_banIPs overflowed at MAX_CVAR_VALUE_STRING\n");
			break;
		}
	}

	trap_cvar_set("g_banIPs", iplist_final);
}

static void add_ip(char *str)
{
	int i;

	for (i = 0; i < num_ip_filters; i++) {
		if (ip_filters[i].compare == 0xffffffff)
			break;
	}

	if (i == num_ip_filters) {
		if (num_ip_filters == MAX_IP_FILTERS) {
			g_printf("IP filter list is full\n");
			return;
		}

		num_ip_filters++;
	}

	if (!string_to_filter(str, &ip_filters[i]))
		ip_filters[i].compare = 0xffffffffu;

	update_ip_bans();
}

void process_ip_bans(void)
{
	char *s, *t;
	char str[MAX_CVAR_VALUE_STRING];

	q_strncpyz(str, g_ban_ips.string, sizeof(str));

	for (t = s = g_ban_ips.string; *t; /* */) {
		s = strchr(s, ' ');
		if (!s)
			break;

		while (*s == ' ')
			*s++ = '\0';

		if (*t)
			add_ip(t);

		t = s;
	}
}

static void svcmd_add_ip(void)
{
	char str[MAX_TOKEN_CHARS];

	if (trap_argc() < 2) {
		g_printf("Usage: addip <ip-mask>\n");
		return;
	}

	trap_argv(1, str, sizeof(str));

	add_ip(str);
}

static void svcmd_remove_ip(void)
{
	struct ip_filter f;
	int i;
	char str[MAX_TOKEN_CHARS];

	if (trap_argc() < 2) {
		g_printf("Usage: removeip <ip-mask>\n");
		return;
	}

	trap_argv(1, str, sizeof(str));

	if (!string_to_filter(str, &f))
		return;

	for (i = 0; i < num_ip_filters; i++) {
		if (ip_filters[i].mask == f.mask &&
			ip_filters[i].compare == f.compare) {
			ip_filters[i].compare = 0xffffffffu;
			g_printf("Removed.\n");

			update_ip_bans();
			return;
		}
	}

	g_printf("Didn't find %s.\n", str);
}

void svcmd_entity_list(void)
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
