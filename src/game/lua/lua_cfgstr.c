#include "lua_local.h"

static int lcfgstr_get(lua_State *l)
{
	int id;
	char value[MAX_STRING_CHARS];

	id = luaL_checkinteger(l, 1);

	trap_get_config_string(id, value, sizeof(value));
	lua_pushstring(l, value);

	return 1;
}

static int lcfgstr_set(lua_State *l)
{
	int id;
	const char *value;

	id = luaL_checkinteger(l, 1);
	value = luaL_checkstring(l, 2);
	if (value == NULL)
		g_error("configstring.set requires a value");

	trap_set_config_string(id, value);

	return 0;
}

static const struct luaL_Reg lcfgstr_functions[] = {
	{ "get", lcfgstr_get },
	{ "set", lcfgstr_set },
	{ NULL, NULL }
};

void lua_init_config_string_library(void)
{
	luaL_newlib(L, lcfgstr_functions);
	lua_pushvalue(L, -1);
	lua_setmetatable(L, -2);
	lua_setglobal(L, "configstring");

	#define CS_GLOBAL(x) \
		do { \
			lua_pushnumber(L, x); \
			lua_setglobal(L, #x); \
		} while (0);

	CS_GLOBAL(CS_SERVER_INFO);
	CS_GLOBAL(CS_SYSTEM_INFO);
	CS_GLOBAL(CS_GAME_VERSION);
	CS_GLOBAL(CS_AMBIENT_PLAY);
	CS_GLOBAL(CS_MESSAGE);
	CS_GLOBAL(CS_SCORES1);
	CS_GLOBAL(CS_SCORES2);
	CS_GLOBAL(CS_WEAPONS);
	CS_GLOBAL(CS_ITEMS);
	CS_GLOBAL(CS_NORTHYAW);
	CS_GLOBAL(CS_FOGVARS);
	CS_GLOBAL(CS_MOTD);
	CS_GLOBAL(CS_VOTE_TIME);
	CS_GLOBAL(CS_VOTE_STRING);
	CS_GLOBAL(CS_VOTE_YES);
	CS_GLOBAL(CS_VOTE_NO);
	CS_GLOBAL(CS_WINNER);
	CS_GLOBAL(CS_STATUS_ICONS);
	CS_GLOBAL(CS_HEAD_ICONS);
	CS_GLOBAL(CS_LOCATIONS);
	CS_GLOBAL(CS_TAG_CONNECTS);
	CS_GLOBAL(CS_CLIENT_VAR_KEYS);
	CS_GLOBAL(CS_CLIENT_VAR_VALUES);
	CS_GLOBAL(CS_MODELS);
	CS_GLOBAL(CS_SOUNDS);
	CS_GLOBAL(CS_FX);
	CS_GLOBAL(CS_FX_ON_TAG);
	CS_GLOBAL(CS_SHELLSHOCKS);
	CS_GLOBAL(CS_MENUS);
	CS_GLOBAL(CS_HINT_STRINGS);
	CS_GLOBAL(CS_LOCALIZED_STRINGS);
	CS_GLOBAL(CS_SHADERS);

	#undef CS_GLOBAL
}