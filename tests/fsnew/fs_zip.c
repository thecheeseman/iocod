#include "fs.h"

static size_t fs_pak_files = 0;
static size_t fs_total_files = 0;

struct pak *fs_load_zip_file(const char *filename, const char *basename)
{
    #ifdef FS_DEBUG
    clock_t start = clock();
    #endif

    mz_zip_archive *zip;
    zip = ic_calloc(1, sizeof(*zip));
    if (zip == NULL) {
        ic_error("error allocating memory for zip structure\n");
        return NULL;
    }

    mz_bool success = mz_zip_reader_init_file(zip, filename, 0);
    if (!success) {
        ic_error("unable to open file: %s\n", filename);
        ic_free(zip);
        return NULL;
    }
    
    uint32_t num_files = mz_zip_reader_get_num_files(zip);
    fs_total_files += num_files;

    /* 
     * sum all filename strlens so we can allocate the names
     * all in one block of memory
     */
    size_t len = 0;
    for (uint32_t i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat stat;

        if (!mz_zip_reader_file_stat(zip, i, &stat)) {
            ic_error("error while stat()\n");
            mz_zip_reader_end(zip);
            ic_free(zip);
            return NULL;
        }

        len += strlen(stat.m_filename) + 1;
    }

    /* allocate blocks of memory for the files in the pk3 + their filenames */
    struct pak_file *buf = 
        ic_malloc((num_files * sizeof(struct pak_file)) + len);
    char *name_ptr = ((char *) buf) + 
        num_files * sizeof(struct pak_file);

    /* find the next largest hash size */
    uint32_t hash_size;
    for (hash_size = 1; hash_size < MAX_FILE_HASH_SIZE; hash_size <<= 1) {
        if (hash_size > num_files)
            break;
    }

    /* allocate pk3 file data */
    struct pak *pak = ic_malloc(sizeof(struct pak) + hash_size * 
                                sizeof(struct pak_file *));
    if (pak == NULL) {
        ic_error("error while allocating memory\n");
        mz_zip_reader_end(zip);
        ic_free(zip);
        return NULL;
    }

    pak->hash_size = hash_size;
    pak->hash_table = 
        (struct pak_file **) (((char *) pak) + sizeof(struct pak));
    for (uint32_t i = 0; i < pak->hash_size; i++)
        pak->hash_table[i] = NULL;

    strncpyz(pak->filename, filename, sizeof(pak->filename));
    strncpyz(pak->basename, basename, sizeof(pak->basename));

    /* strip extension if needed */
    size_t bnamelen = strlen(pak->basename);
    if (bnamelen > 4 && !strcasecmp(pak->basename + (bnamelen - 4), ".pk3"))
        pak->basename[bnamelen - 4] = '\0';

    pak->handle = zip;
    pak->num_files = num_files;

    /* load file data into structures */
    for (uint32_t i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat stat;
        char fname[MAX_PATH_LEN];

        if (!mz_zip_reader_file_stat(zip, i, &stat)) {
            ic_error("error while stat()\n");
            mz_zip_reader_end(zip);
            ic_free(zip);
            return NULL;
        }

        strncpy(fname, stat.m_filename, sizeof(fname));
        strlwr(fname);

        uint32_t hash = fs_hash_filename(fname, pak->hash_size);

        buf[i].name = name_ptr;
        strcpy(buf[i].name, fname);
        name_ptr += strlen(fname) + 1;

        buf[i].pos = stat.m_central_dir_ofs;

        buf[i].next = pak->hash_table[hash];
        pak->hash_table[hash] = &buf[i];
    }

    #ifdef FS_DEBUG
    clock_t end = clock();
    ic_printf("time elapsed: %f\n",
              ((double) (end - start) / CLOCKS_PER_SEC));
    #endif

    pak->buf = buf;
    return pak;
}
