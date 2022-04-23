#include "fs_local.h"
#include "strings/stringlib.h"

struct fs fs;

void fs_init(void)
{
    com_startup_variable("fs_cdpath");
    com_startup_variable("fs_basepath");
    com_startup_variable("fs_homepath");
    com_startup_variable("fs_game");
    com_startup_variable("fs_copyfiles");
    com_startup_variable("fs_restrict");
    com_startup_variable("fs_usewolf");
    com_startup_variable("cl_language");

    fs_startup(BASEGAME);

    // cl_init_language_cvars();

    // FUN_004aaaa0(&DAT_01407450, 0);
    // FUN_004a9b10(); // language load? localized assets

    fs_set_restrictions();

    if (fs_read_file("default.cfg", NULL) <= 0) {
        com_error(ERR_FATAL, "Couldn't load default.cfg. " \
                  "Make sure iocod is run from the correct folder.");
    }

    strncpyz(fs.last_valid_base, fs.base_path->string, sizeof(fs.last_valid_base));
    strncpyz(fs.last_valid_game, fs.game_dir_cvar->string, sizeof(fs.last_valid_game));
}
