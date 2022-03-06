#include "script_local.h"

char *get_gametype_name_for_script(const char *str)
{
	int i;
	struct gametype *gt;

	for (i = 0; i < g_scr_data.num_gametypes; i++) {
		gt = &g_scr_data.gametypes[i];

		if (q_stricmp(gt->name, str) == 0)
			return gt->description;
	}

	return NULL;
}

bool is_valid_gametype(const char *gametype)
{
	return (get_gametype_name_for_script(gametype) != NULL);
}
