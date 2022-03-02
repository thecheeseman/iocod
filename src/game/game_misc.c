#include "game_local.h"

bool g_get_hint_string_index(int *hint, char *string)
{
	int i;
	char s[MAX_STRING_CHARS];

	for (i = 0; i < MAX_CONFIG_STRINGS; i++) {
		trap_get_config_string(CS_HINT_STRINGS + i, s, sizeof(s));
		if (!s[0])
			break;

		if (!strcmp(s, string)) {
			*hint = i;
			return true;
		}
	}

	if (i == MAX_CONFIG_STRINGS) {
		*hint = -1;
		return false;
	}

	trap_set_config_string(CS_HINT_STRINGS + i, string);
	*hint = i;
	return true;
}