#include "lua_local.h"

lua_State *L;

#define SCRIPT_DIR "luascripts"
#define MAPS_DIR "luascripts/maps"
#define GAMETYPES_DIR "luascripts/gametypes"

static void print_error(int code)
{
	const char *type;

	switch (code) {
	case LUA_ERRRUN: type = "runtime error"; break;
	case LUA_ERRSYNTAX: type = "syntax error"; break;
	default: type = ""; break;
	}

	g_error("%s: %s", type, lua_tostring(L, -1), code);
	lua_pop(L, 1); // pop error
}

static int load_script(const char *name)
{
	int ret;
	
	ret = luaL_loadfile(L, va("%s/%s/%s.lua", BASEGAME, SCRIPT_DIR, name));
	if (ret != LUA_OK) {
		print_error(ret);
		return ret;
	 }
	
	ret = lua_pcall(L, 0, 0, 0);
	if (ret != LUA_OK) {
		print_error(ret);
		return ret;
	}

	return LUA_OK;
}

static int call_global_function(const char *name)
{
	int ret;

	lua_getglobal(L, name);
	ret = lua_pcall(L, 0, 0, 0);

	if (ret != LUA_OK) {
		print_error(ret);
		return ret;
	}

	return LUA_OK;
}

void scr_load_gametype(void)
{

}

void scr_load_level(void)
{

}

void scr_startup_gametype(void)
{

}

void lua_initialise(void)
{
	// init
	L = luaL_newstate();
	luaL_openlibs(L);

	// libraries
	lua_init_cvar_library();
	lua_init_config_string_library();

	g_printf("got %d gametypes\n", g_scr_data.num_gametypes);

	//
	load_script("main");
	call_global_function("init");
}

void lua_shutdown(void)
{
	lua_close(L);
}