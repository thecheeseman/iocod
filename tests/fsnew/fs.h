#ifndef FS_H
#define FS_H

#define FS_DEBUG
#include <stdio.h>
#include "../icheaders/iocod.h"
#include "miniz.h"

#define FS_OK  (0)
#define FS_ERR (1)

#define MAX_FILE_HASH_SIZE  4096    /// total number of files supported in pk3
#define MAX_FILENAME_LEN    72      /// maximum filename length
#define MAX_PATH_LEN        256     /// maximum path length

#define MAX_SEARCH_PATHS    4096
#define MAX_FILE_HANDLES    4096

#define FS_BASE_GAME        "main"

/**
 * @brief File in .pk3
*/
struct pak_file {
    char *name;
    uint64_t pos;
    struct pak_file *next;
};

/**
 * @brief .pk3
*/
struct pak {
    char filename[MAX_FILENAME_LEN];
    char basename[MAX_FILENAME_LEN];
    char gamename[MAX_FILENAME_LEN];

    mz_zip_archive *handle;

    int checksum;
    int pure_checksum;
    uint32_t num_files;
    uint32_t hash_size;

    struct pak_file **hash_table;
    struct pak_file *buf;
};

struct directory {
    char path[MAX_FILENAME_LEN];
    char game_path[MAX_FILENAME_LEN];
};

struct search_path {
    struct search_path *next;

    struct pak *pak;
    struct directory *dir;
};

struct file_handle {
    struct {
        union {
            FILE *o;
            mz_zip_archive *z;
        } file;

        bool unique;
    } handle;

    bool handle_sync;

    size_t base_offset;
    size_t file_size;

    bool zip_file;
    size_t zip_file_pos;

    bool streamed;

    char name[MAX_PATH_LEN];
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

    char game_dir[MAX_FILENAME_LEN];
    char last_valid_base[MAX_FILENAME_LEN];
    char last_valid_game[MAX_FILENAME_LEN];
};

extern struct fs fs;

IC_PUBLIC
void fs_init(void);

/**
 * @brief File name hash from Q3
 * @param filename
 * @param size
 * @return
*/
uint32_t fs_hash_filename(const char *filename, int size);

/**
 * @brief 
 * @param filename 
 * @param base_name 
 * @return 
*/
struct pak *fs_load_zip_file(const char *filename, const char *basename);

/**
 * @brief Get list of files in a given directory.
 * @param dir           directory to search
 * @param ext           file extension to filter by
 * @param filter        if not NULL, will cause a recursive search
 * @param want_subdirs  true if you want a list of directories instead of files
 * @param num_files     returned number of files
 * @return              a char array of files, or NULL
*/
IC_PUBLIC
char **fs_list_files(const char *dir, const char *ext, char *filter,
                     bool want_subdirs, size_t *num_files);

IC_PUBLIC
void fs_free_file_list(char **list);

#endif /* FS_H */
