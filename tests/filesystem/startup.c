#include "fs_local.h"
#include "strings/stringlib.h"

#include "load_folder_data.h"

#if defined PLATFORM_LINUX || defined PLATFORM_MACOS
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

// merged here from system/ because 
// this is always the current working dir anyway
static char *default_install_path(void)
{
    return fs_getcwd();
}

static char homepath[MAX_PATH];
static char *default_home_path(void)
{
    #ifdef PLATFORM_WINDOWS
    return fs_getcwd();
    #else
    char *p;

    if ((p = getenv("HOME")) != NULL) {
        strncpyz(homepath, p, sizeof(homepath));
        strncatz(homepath, sizeof(homepath), "/.callofduty");

        if (mkdir(homepath, 0777)) {
            if (errno != EEXIST)
                com_error(ERR_FATAL, "Unable to create directory /.callofduty");
        }

        return homepath;
    }

    return "";
    #endif
}

static void add_path(const char *game_name)
{
    if (*fs.base_path->string != '\0') {
        fs_add_game_directory(fs.base_path->string, game_name);

        /* linux can have a different directory structure */
        if (*fs.home_path->string != '\0' &&
            strncasecmp(fs.home_path->string,
                        fs.base_path->string, MAX_PATH) != 0) {
            fs_add_game_directory(fs.home_path->string, game_name);
        }
    }
}

void fs_startup(const char *game_name)
{
    com_print_header("fs_startup", 20, '-');

    fs.debug =      cvar_get("fs_debug", "1", 0);
    fs.copy_files = cvar_get("fs_copyfiles", "0", CVAR_INIT);
    fs.base_path =  cvar_get("fs_basepath", default_install_path(), CVAR_INIT);
    fs.base_game =  cvar_get("fs_basegame", "", CVAR_INIT);

    const char *homepath = default_home_path();
    if (homepath == NULL || *homepath == '\0')
        homepath = fs.base_path->string;

    fs.home_path =          cvar_get("fs_homepath", homepath, CVAR_INIT);
    fs.game_dir_cvar =      cvar_get("fs_game", "", CVAR_INIT | CVAR_SYSTEM_INFO);
    fs.restrict_cvar =      cvar_get("fs_restrict", "", CVAR_INIT);
    fs.ignore_localized =   cvar_get("fs_ignorelocalized", "0",
                                     CVAR_CHEAT | CVAR_LATCH);

    add_path(game_name);

    /* check for additional mods */
    if (*fs.base_game->string != '\0' &&
        strcasecmp(game_name, BASEGAME) == 0 &&
        strcasecmp(fs.base_game->string, game_name) != 0) {
        add_path(fs.base_game->string);
    }

    /* check additional game folder for mods */
    if (*fs.game_dir_cvar->string != '\0' &&
        strcasecmp(game_name, BASEGAME) == 0 &&
        strcasecmp(fs.game_dir_cvar->string, game_name) != 0) {
        add_path(fs.game_dir_cvar->string);
    }

    fs_load_folder_data(FDT_XANIM);
    fs_load_folder_data(FDT_XMODEL);
    fs_load_folder_data(FDT_XMODELPARTS);
    fs_load_folder_data(FDT_XMODELSURFS);
    fs_load_folder_data(FDT_WEAPONS);
    fs_load_folder_data(FDT_ANIMTREES);
}
