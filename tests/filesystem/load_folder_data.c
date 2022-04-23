#include "fs_local.h"
#include "load_folder_data.h"

struct fdt_data {
    enum folder_data_type type;
    const char *name;
    const char *ext;
};

static struct fdt_data data[] = {
    [FDT_NONE] = {
        .type = FDT_NONE,
        .name = "none",
        .ext = ""
    },
    [FDT_ANIMTREES] = {
        .type = FDT_ANIMTREES,
        .name = "animtrees",
        .ext = ".atr"
    },
    [FDT_XANIM] = {
        .type = FDT_XANIM,
        .name = "xanim",
        .ext = ""
    },
    [FDT_XMODEL] = {
        .type = FDT_XMODEL,
        .name = "xmodel",
        .ext = ""
    },
    [FDT_XMODELPARTS] = {
        .type = FDT_XMODELPARTS,
        .name = "xmodelparts",
        .ext = ""
    },
    [FDT_XMODELSURFS] = {
        .type = FDT_XMODELSURFS,
        .name = "xmodelsurfs",
        .ext = ""
    },
    [FDT_WEAPONS] = {
        .type = FDT_WEAPONS,
        .name = "weapons",
        .ext = ""
    },
    [FDT_END] = {
        .type = FDT_END,
        .name = NULL,
        .ext = NULL
    }
};

static void load_folder_data(const char *path, const char *dir,
                             const char *folder, const char *ext)
{

}

void fs_load_folder_data(enum folder_data_type folder)
{
    if (folder > FDT_END || folder <= FDT_NONE)
        return;

    for (struct search_path *sp = fs.search_paths; sp != NULL; sp = sp->next) {
        if (sp->pak == NULL) {
            load_folder_data(sp->dir->path,
                             sp->dir->game_dir,
                             data[folder].name,
                             data[folder].ext);
        }
    }
}
