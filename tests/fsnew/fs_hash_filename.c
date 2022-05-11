#include <ctype.h>
#include "fs.h"

uint32_t fs_hash_filename(const char *filename, int size)
{
    long hash = 0;
    int i = 0;
    while (filename[i] != '\0') {
        char c = tolower(filename[i]);

        if (c == '.')
            break; /* ignore extensions */

        if (c == '\\' || c == '/')
            c = '/'; /* force Windows to conform */

        hash += (long) (c) * (i + 119);
        i++;
    }

    hash = (hash ^ (hash >> 10) ^ (hash >> 20));
    hash &= (size - 1);
    return hash;
}
