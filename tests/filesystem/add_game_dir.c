#include "fs_local.h"
#include "strings/stringlib.h"

static void update_search_paths(struct search_path *sp)
{
    struct search_path *p;

    if (fs.search_paths == NULL) {
        sp->next = fs.search_paths;
        fs.search_paths = sp;
    } else {
        // loop through search paths, find next empty slot
        for (p = fs.search_paths; p->next != NULL; p = p->next);

        sp->next = p->next;
        p->next = sp;
    }
}

static int path_compare(const char *s1, const char *s2)
{
    int c1, c2;

    do {
        c1 = *s1++;
        c2 = *s2++;

        if (c1 >= 'a' && c1 <= 'z')
            c1 -= ('a' - 'A');
        if (c2 >= 'a' && c2 <= 'z')
            c2 -= ('a' - 'A');

        if (c1 == '\\' || c1 == ':')
            c1 = '/';
        if (c2 == '\\' || c2 == ':')
            c2 = '/';

        if (c1 < c2)
            return -1;      // strings not equal

        if (c1 > c2)
            return 1;

    } while (c1);

    return 0;       // strings are equal
}

static char *get_localized_language_name(const char *pak_name)
{
    static int id = 0;
    static char paks[2][MAX_PATH]; // hack

    if (strnlen(pak_name, MAX_PATH) < 10) {
        paks[id][0] = '\0';
    } else {
        memset(paks[id], 0, MAX_PATH);

        for (int i = 10;
             i < MAX_PATH && pak_name[i] != '\0' && pak_name[i] != '_'; i++) {
            paks[id][i - 10] = pak_name[i];
        }
    }

    return paks[id];
}

static int pak_sort(const void *a, const void *b)
{
    char *aa;
    char *bb;
    char *l1;
    char *l2;

    aa = *(char **) a;
    bb = *(char **) b;

    if (strncmp(aa, "          ", 10) == 0 &&
        strncmp(bb, "          ", 10) == 0) {
        l1 = get_localized_language_name(aa);
        l2 = get_localized_language_name(bb);

        if (strcasecmp(l1, "english") == 0 &&
            strcasecmp(l2, "english") != 0) {
            return -1;
        } else if (strcasecmp(l1, "english") != 0 &&
                   strcasecmp(l2, "english") == 0) {
            return 1;
        }
    }

    return path_compare(aa, bb);
}

#define MAX_PAK_FILES 65536 // ridiculous number of pak files to be safe
static void find_pak_files(const char *path, const char *dir)
{
    char ospath[MAX_OSPATH];
    char **pak_files;
    char *sorted[MAX_PAK_FILES];
    size_t num_files = 0;

    fs_build_path(path, dir, "", ospath);

    com_printf("build_path: %s\n", ospath);

    #if 0

    pak_files = fs_list_files(ospath, ".pk3", NULL, &num_files, false);
    if (num_files > MAX_PAK_FILES) {
        com_warning("Exceeded max number of pak files (%d) in %s\n",
                    MAX_PAK_FILES, ospath);
        num_files = MAX_PAK_FILES; // clamp at max
    }

    for (size_t i = 0; i < num_files; i++) {
        sorted[i] = pak_files[i];

        // hack to force localized files to be sorted last
        if (strncmp(sorted[i], "localized_", 10) == 0)
            memcpy(sorted[i], "          ", 10);
    }

    qsort(sorted, num_files, sizeof(char *), pak_sort);

    fs_free_file_list(pak_files);
    #endif
}

void fs_add_game_directory(const char *path, const char *dir)
{
    struct search_path *sp;

    char newdir[MAX_OSPATH];
    strncpyz(newdir, dir, sizeof(newdir));
    strncpyz(fs.game_dir, newdir, sizeof(newdir));

    sp = z_malloc(sizeof(struct search_path));
    sp->dir = z_malloc(sizeof(struct directory));
    strncpyz(sp->dir->path, path, sizeof(sp->dir->path));
    strncpyz(sp->dir->game_dir, newdir, sizeof(sp->dir->game_dir));

    update_search_paths(sp);
    find_pak_files(path, newdir);
}
