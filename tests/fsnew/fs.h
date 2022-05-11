#ifndef FS_H
#define FS_H

#define FS_DEBUG
#include <stdio.h>

#ifdef FS_DEBUG
#include <time.h>
#endif

#include "../icheaders/iocod.h"
#include "miniz.h"

#define FS_OK  (0)
#define FS_ERR (1)


#define MAX_FILE_HASH_SIZE  4096    /// total number of files supported in pk3
#define MAX_FILENAME_LEN    64      /// maximum filename length
#define MAX_PATH_LEN        256     /// maximum path length

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

#endif /* FS_H */
