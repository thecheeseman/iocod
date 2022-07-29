/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

#include "fs_local.h"
#include <ctype.h>

#define HASH_Q3

IC_NON_NULL(1)
u32 hash_filename(_In_z_ const char *filename, 
                  u32 hash_size)
{
    u32 hash = 0x12345678;

    #ifdef HASH_Q3 // 24123 collisions
    u32 i = 0;
    while (filename[i] != '\0') {
        char c = tolower(filename[i]);

        if (c == '.')
            break;

        if (c == '\\' || c == '/')
            c = '/';

        hash += (long) (c) * (i + 119);
        i++;
    }

    hash = (hash ^ (hash >> 10) ^ (hash >> 20));
    #elif defined HASH_DJB2 // 23771 collisions
    int c;

    while ((c = *filename++) != '\0')
        hash = ((hash << 5) + hash) + c;
    #elif defined HASH_JENKINS // 23710
    size_t len = strlen(filename);
    for (size_t i = 0; i < len; ++i) {
        hash += filename[i];
        hash += (hash << 10);
        hash ^= (hash >> 3);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    #elif defined HASH_MURMUR32 // 23609
    for (; *filename != '\0'; ++filename) {
        hash ^= *filename;
        hash *= 0x5bd1e995;
        hash ^= hash >> 15;
    }
    #elif defined HASH_FNV // 23810
    size_t len = strlen(filename);
    hash ^= 2166136261UL;
    for (size_t i = 0; i < len; ++i) {
        hash ^= filename[i]; 
        hash *= 16777619;
    }
    #endif

    return (hash & (hash_size - 1));
}
