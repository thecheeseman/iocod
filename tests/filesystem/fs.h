#ifndef FS_H
#define FS_H

#include "types/bool.h"

#define fs_set_restrictions()

// build_path.c
void fs_build_path(const char *base, const char *game,
                           const char *path, char *buffer);
void test_fs_build_path(void);

// create_path.c
bool fs_create_path(const char *cpath);
void test_fs_create_path(void);

// get_cwd.c
char *fs_getcwd(void);

// init.c
void fs_init(void);

// list_files.c
void test_fs_file_list(void);

// replace_separators.c
void fs_replace_separators(char *path);
void test_fs_replace_separators(void);

// startup.c
void fs_startup(const char *game_name);

// valid_path.c
bool fs_is_valid_path(const char *path);
void test_fs_is_valid_path(void);

#endif /* FS_H */
