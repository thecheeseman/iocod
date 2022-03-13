#include "lua_local.h"

static int lcvar_get(lua_State *l)
{
	const char *name;
	char value[MAX_STRING_CHARS];

	name = luaL_checkstring(l, 1);
	if (name == NULL)
		g_error("name cannot be empty");

	trap_cvar_variable_string_buffer(name, value, sizeof(value));
	lua_pushstring(l, value);

	return 1;
}

static int lcvar_set(lua_State *l)
{
	const char *name, *value;

	name = luaL_checkstring(l, 1);
	value = luaL_checkstring(l, 2);
	if (name == NULL)
		g_error("cvar.set requires a name");
	if (value == NULL)
		g_error("cvar.set requires a value");

	trap_cvar_set(name, value);

	return 0;
}

static const struct luaL_Reg lcvar_functions[] = {
	{ "get", lcvar_get },
	{ "set", lcvar_set },
	{ NULL, NULL }
};

void lua_init_cvar_library(void)
{
	// cvar functions
	luaL_newlib(L, lcvar_functions);
	lua_pushvalue(L, -1);
	lua_setmetatable(L, -2);
	lua_setglobal(L, "cvar");
}