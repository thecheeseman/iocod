#ifndef FS_LOCAL_H
#define FS_LOCAL_H

#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "iocod.h"
#include "cvar/cvar.h"
#include "types/bool.h"
#include "fs.h"

// for local testing
#ifdef FS_STANDALONE
typedef struct {
    int a;
} unzFile;
#endif

#define BASEGAME "main"

#define MAX_OSPATH 72
#define MAX_PATH 256

#define MAX_SEARCH_PATHS 4096
#define MAX_FILE_HANDLES 1024

struct file_in_pak {
    char *name;
    size_t pos;
    struct file_in_pak *next;
};

struct pak_file {
    char file_name[MAX_OSPATH];
    char base_name[MAX_OSPATH];
    char game_name[MAX_OSPATH];

    unzFile handle;

    int checksum;
    int pure_checksum;
    int num_files;

    int referenced;

    int hash_size;

    struct file_in_pak **hash_table;
    struct file_in_pak *build_buffer;
};

struct directory {
    char path[MAX_OSPATH];
    char game_dir[MAX_OSPATH];
};

struct search_path {
    struct search_path *next;

    struct pak_file *pak;
    struct directory *dir;

    bool localized;
    int language;
};

union qfile_gu {
    FILE *o;
    unzFile z;
};

struct qfile_u {
    union qfile_gu file;
    bool unique;
};

struct file_handle {
    struct qfile_u handle;
    bool handle_sync;

    size_t base_offset;
    size_t file_size;

    bool zip_file;
    size_t zip_file_pos;

    bool streamed;

    char name[MAX_PATH];
};

struct fs {
    struct cvar *debug;
    struct cvar *copy_files;
    struct cvar *base_path;
    struct cvar *base_game;
    struct cvar *home_path;
    struct cvar *game_dir_cvar;
    struct cvar *restrict_cvar;
    struct cvar *ignore_localized;

    struct search_path *search_paths;

    struct file_handle handles[MAX_FILE_HANDLES];

    int num_paks;   /// total number of pk3s 
    int num_files;  /// total number of files registered

    int num_server_paks;
    int server_pak_checksums[MAX_SEARCH_PATHS];
    int server_pak_names[MAX_SEARCH_PATHS];

    int num_referenced_paks;
    int referenced_pak_checksums[MAX_SEARCH_PATHS];
    int referenced_pak_names[MAX_SEARCH_PATHS];

    char game_dir[MAX_OSPATH];
    char last_valid_base[MAX_OSPATH];
    char last_valid_game[MAX_OSPATH];
};

extern struct fs fs;

void fs_add_game_directory(const char *path, const char *dir);

#endif /* FS_LOCAL_H */
