#include "fs.h"

/**
 * @brief Update search paths
 * @param sp search path to add to the list
*/
static void update_search_paths(struct search_path *sp)
{
    struct search_path *p;

    if (fs.search_paths == NULL) {
        sp->next = fs.search_paths;
        fs.search_paths = sp;
    }
    else {
        for (p = fs.search_paths; p->next != NULL; p = p->next) {
            /* loop through paths, find the next empty slot */
        }

        sp->next = p->next;
        p->next = sp;
    }
}

static void find_pak_files(const char *path, const char *dir)
{

}

IC_PUBLIC
fs_add_game_directory(const char *path, const char *dir)
{
    struct search_path *sp;

    char newdir[MAX_FILENAME_LEN];
    strncpyz(newdir, dir, sizeof(newdir));
    // strncpyz(fs.game_dir, newdir, sizeof(newdir));

    sp = ic_malloc(sizeof(*sp));
    sp->dir = ic_malloc(sizeof(*sp->dir));
    strncpyz(sp->dir->path, path, sizeof(sp->dir->path));
    strncpyz(sp->dir->game_path, newdir, sizeof(sp->dir->game_path));

    update_search_paths(sp);
    find_pak_files(path, newdir);
}
