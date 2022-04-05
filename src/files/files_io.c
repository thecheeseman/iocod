/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

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

#include "files_local.h"
#include "common/error.h"
#include "common/hunk.h"
#include "common/memory.h"
#include "common/print.h"
#include "cvar/cvar.h"
#include "strings/stringlib.h"

/**
 * @brief Copy a fully specified file from one place to another
 * @param fromospath
 * @param toospath
*/
void fs_copy_file(char *fromospath, char *toospath)
{
    FILE *f;
    long len;
    byte *buf;

    com_printf("copy %s to %s\n", fromospath, toospath);

    if (strstr(fromospath, "journal.dat") || 
        strstr(fromospath, "journaldata.dat")) {
        com_printf("ignoring journal files\n");
        return;
    }

    f = fopen(fromospath, "rb");
    if (f == NULL)
        return;

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    buf = z_malloc(len);
    if ((long) fread(buf, 1, len, f) != len)
        com_error(ERR_FATAL, "short read in fs_copy_files()");
    fclose(f);

    if (fs_create_path(toospath))
        return;

    f = fopen(toospath, "wb");
    if (f == NULL) {
        z_free(buf);
        return;
    }

    if (fwrite(buf, 1, len, f) != 1)
        com_error(ERR_FATAL, "short write in fs_copy_files()");

    fclose(f);
    z_free(buf);
}

/**
 * @brief
 * @param ospath
*/
void fs_remove(const char *ospath)
{
    remove(ospath);
}

/**
 * @brief
 * @param file
 * @return
*/
bool fs_file_exists(const char *file)
{
    FILE *f;
    char testpath[MAX_OSPATH];

    fs_build_ospath(fs_homepath->string, fs_gamedir, file, testpath);

    f = fopen(testpath, "rb");
    if (f != NULL) {
        fclose(f);
        return true;
    }

    return false;
}

/**
 * @brief
 * @param from
 * @param to
*/
void fs_rename(const char *from, const char *to)
{
    char from_ospath[MAX_OSPATH], to_ospath[MAX_OSPATH];

    if (fs_searchpaths == NULL)
        com_error(ERR_FATAL, "Filesystem call made without initialization");

    // don't let sound stutter -- useless on server?
    s_clear_sound_buffer();

    fs_build_ospath(fs_homepath->string, from, "", from_ospath);
    fs_build_ospath(fs_homepath->string, to, "", to_ospath);
    FS_DEBUG_PRINT("%s -> %s\n", from_ospath, to_ospath);

    if (rename(from_ospath, to_ospath)) {
        // failed to rename, try copying and deleting the original
        fs_copy_file(from_ospath, to_ospath);
        fs_remove(from_ospath);
    }
}

/**
 * @brief Close the given file. If the file is an open pk3 file, leave it open
 * @param f
*/
void fs_fclose_file(filehandle f)
{
    if (fs_searchpaths == NULL)
        com_error(ERR_FATAL, "Filesystem call made without initialization");

    if (fsh[f].streamed)
        sys_end_streamed_file(f);

    if (fsh[f].zipfile) {
        unzCloseCurrentFile(fsh[f].handlefiles.file.z);
        if (fsh[f].handlefiles.unique)
            unzClose(fsh[f].handlefiles.file.z);

        com_memset(&fsh[f], 0, sizeof(fsh[f]));
        return;
    }

    // we didn't find it as a pk3, so close it as a unique file
    if (fsh[f].handlefiles.file.o != NULL)
        fclose(fsh[f].handlefiles.file.o);

    com_memset(&fsh[f], 0, sizeof(fsh[f]));
}

/**
 * @brief
 * @param filename
 * @return
*/
filehandle fs_fopen_file_write(const char *filename)
{
    char ospath[MAX_OSPATH];
    filehandle f;

    //if (!fs_searchpaths)
    //	com_error(ERR_FATAL, "Filesystem call made without initialization");

    f = fs_handle_for_file();
    fsh[f].zipfile = false;

    fs_build_ospath(fs_homepath->string, fs_gamedir, filename, ospath);
    FS_DEBUG_PRINT("%s\n", ospath);

    if (fs_create_path(ospath))
        return 0;

    fsh[f].handlefiles.file.o = fopen(ospath, "wb");

    strncpyz(fsh[f].name, filename, sizeof(fsh[f].name));

    fsh[f].handlesync = false;
    if (fsh[f].handlefiles.file.o == NULL)
        f = 0;

    return f;
}

/**
 * @brief
 * @param filename
 * @return
*/
filehandle fs_fopen_file_append(const char *filename)
{
    char ospath[MAX_OSPATH];
    filehandle f;

    //if (!fs_searchpaths)
    //	com_error(ERR_FATAL, "Filesystem call made without initialization");

    f = fs_handle_for_file();
    fsh[f].zipfile = false;

    strncpyz(fsh[f].name, filename, sizeof(fsh[f].name));

    // don't let sound stutter -- useless on server?
    s_clear_sound_buffer();

    fs_build_ospath(fs_homepath->string, fs_gamedir, filename, ospath);
    FS_DEBUG_PRINT("%s\n", ospath);

    if (fs_create_path(ospath))
        return 0;

    fsh[f].handlefiles.file.o = fopen(ospath, "ab");

    strncpyz(fsh[f].name, filename, sizeof(fsh[f].name));

    fsh[f].handlesync = false;
    if (fsh[f].handlefiles.file.o == NULL)
        f = 0;

    return f;
}

// i think this is a bug
#define DEMOEXT ".dm_NETWORK_PROTOCOL_VERSION"

/**
 * @brief
 * @param filename
 * @param file
 * @param uniquefile
 * @param a no idea what this is
 * @return
*/
int fs_fopen_file_read(const char *filename, filehandle *file, 
                       bool uniquefile, int a)
{
    struct searchpath *search;
    uint32_t hash = 0;
    struct pack *pak, *unpurepak;
    struct fileinpack *pakfile;
    struct directory *dir;
    char ospath[MAX_OSPATH];
    FILE *temp;
    int l;
    unz_s *zfi;

    //if (!fs_searchpaths)
    //	com_error(ERR_FATAL, "Filesystem call made without initialization");

    if (file == NULL) {
        // just want to see if file is there
        for (search = fs_searchpaths; search != NULL; search = search->next) {
            if (!fs_search_localized(search))
                continue;

            if (search->pack != NULL)
                hash = fs_hash_filename(filename, search->pack->hashsize);

            // do we have a pk3 file
            if (search->pack != NULL && search->pack->hashtable[hash]) {
                // search through all the pk3 files
                pak = search->pack;
                pakfile = pak->hashtable[hash];
                do {
                    if (!fs_filename_compare(pakfile->name, filename))
                        return true;

                    pakfile = pakfile->next;
                } while (pakfile != NULL);
            } else if (search->dir != NULL) {
                dir = search->dir;

                fs_build_ospath(dir->path, dir->gamedir, filename, ospath);
                temp = fopen(ospath, "rb");
                if (temp == NULL)
                    continue;

                fclose(temp);
                return true;
            }
        }

        return false;
    }

    // qpaths are not supposed to have a leading slash
    if (filename[0] == '/' || filename[0] == '\\')
        filename++;

    // make absolutely sure that it can't back up the path.
    // The searchpaths do guarantee that something will always
    // be prepended, so we don't need to worry about "c:" or "//limbo"
    if (strstr(filename, "..") != NULL || strstr(filename, "::") != NULL) {
        *file = 0;
        return -1;
    }

    // no check for codkey here

    // search through the path, one element at a time
    *file = fs_handle_for_file();
    fsh[*file].handlefiles.unique = uniquefile;

    unpurepak = NULL;
    for (search = fs_searchpaths; search != NULL; search = search->next) {
        if (!fs_search_localized(search))
            continue;

        pak = search->pack;
        if (pak != NULL)
            hash = fs_hash_filename(filename, pak->hashsize);

        if (pak != NULL && pak->hashtable[hash]) {
            pakfile = pak->hashtable[hash];

            do {
                // found it
                if (!fs_filename_compare(pakfile->name, filename)) {
                    if (search->localized || fs_pak_is_pure(pak)) {
                        l = strlen(filename);

                        // mark the pak as having been referenced and mark specifics on cgame and ui
                        // shaders, txt, arena files  by themselves do not count as a reference as
                        // these are loaded from all pk3s
                        // from every pk3 file..
                        if (!(pak->referenced & FS_GENERAL_REF)) {
                            if (strcasecmp(filename + l - 7, ".shader") &&
                                strcasecmp(filename + l - 4, ".txt") &&
                                strcasecmp(filename + l - 4, ".cfg") &&
                                strcasecmp(filename + l - 7, ".config") &&
                                strstr(filename, "levelshots") == NULL &&
                                strcasecmp(filename + l - 6, ".arena") &&
                                strcasecmp(filename + l - 5, ".menu")) {
                                pak->referenced |= FS_GENERAL_REF;
                            }
                        }

                        if (!(pak->referenced & FS_QAGAME_REF) &&
                            fs_shifted_strstr(filename, SYS_DLLNAME_QAGAME,
                                              -SYS_DLLNAME_QAGAME_SHIFT)) {
                            pak->referenced |= FS_QAGAME_REF;
                        }

                        if (!(pak->referenced & FS_CGAME_REF) &&
                            fs_shifted_strstr(filename, SYS_DLLNAME_CGAME,
                                              -SYS_DLLNAME_CGAME_SHIFT)) {
                            pak->referenced |= FS_CGAME_REF;
                        }

                        if (!(pak->referenced & FS_UI_REF) &&
                            fs_shifted_strstr(filename, SYS_DLLNAME_UI,
                                              -SYS_DLLNAME_UI_SHIFT)) {
                            pak->referenced |= FS_UI_REF;
                        }

                        if (uniquefile) {
                            fsh[*file].handlefiles.file.z =
                                unzReOpen(pak->pak_filename, pak->handle);

                            if (fsh[*file].handlefiles.file.z == NULL) {
                                if (a) {
                                    // TODO: whatever this is
                                    // fun_080612a2(*file);
                                    *file = 0;
                                    return -1;
                                }

                                com_error(ERR_FATAL, "Couldn't reopen %s",
                                          pak->pak_filename);
                            }
                        } else {
                            fsh[*file].handlefiles.file.z = pak->handle;
                        }

                        strncpyz(fsh[*file].name, filename,
                                   sizeof(fsh[*file].name));
                        fsh[*file].zipfile = true;

                        zfi = (unz_s *) fsh[*file].handlefiles.file.z;

                        // in case the file was new
                        temp = zfi->file;

                        // set the file position in the zip file (also sets the current file info)
                        unzSetCurrentFileInfoPosition(pak->handle, pakfile->pos);

                        // copy the file info into the unzip structure
                        com_memcpy(zfi, pak->handle, sizeof(unz_s));

                        // we copy this back into the structure
                        zfi->file = temp;

                        // open the file in the zip
                        unzOpenCurrentFile(fsh[*file].handlefiles.file.z);

                        fsh[*file].zipfilepos = pakfile->pos;

                        FS_DEBUG_PRINT("%s (found in '%s)\n", filename,
                                       pak->pak_filename);

                        return zfi->cur_file_info.uncompressed_size;
                    }

                    unpurepak = pak;
                    break;
                }

                pakfile = pakfile->next;
            } while (pakfile != NULL);
        } else if (search->dir != NULL) {
            // check directory tree
            l = strlen(filename);

            // if we are running restricted, or if the filesystem is configured for pure
            // the only files we allow are these
            if (fs_restrict->integer > 0 || fs_num_serverpaks > 0) {
                if (strcasecmp(filename + l - 4, ".cfg") != 0 &&
                    strcasecmp(filename + l - 5, ".menu") != 0 &&
                    strcasecmp(filename + l - strlen(DEMOEXT), DEMOEXT) != 0 &&
                    strcasecmp(filename + l - 4, ".dat") != 0) {
                    continue;
                }
            }

            dir = search->dir;
            fs_build_ospath(dir->path, dir->gamedir, filename, ospath);
            fsh[*file].handlefiles.file.o = fopen(ospath, "rb");
            if (fsh[*file].handlefiles.file.o == NULL)
                continue;

            if (strcasecmp(filename + l - 4, ".cfg") != 0 &&
                strcasecmp(filename + l - 5, ".menu") != 0 &&
                strcasecmp(filename + l - strlen(DEMOEXT), DEMOEXT) != 0 &&
                strcasecmp(filename + l - 4, ".dat") != 0) {
                fs_fake_checksum = random();
            }

            strncpyz(fsh[*file].name, filename, sizeof(fsh[*file].name));
            fsh[*file].zipfile = false;

            FS_DEBUG_PRINT("%s (found in '%s/%s')\n", filename, dir->path, 
                           dir->gamedir);

            // if we are getting it from the cdpath, copy to the basepath
            if (fs_copyfiles->integer > 0 && 
                strcasecmp(dir->path,fs_cdpath->string) == 0) {
                char newpath[MAX_OSPATH];
                fs_build_ospath(fs_basepath->string, dir->gamedir, filename, 
                                newpath);
                fs_copy_file(ospath, newpath);
            }

            return fs_file_length(*file);
        }

        // not really necessary on server but its here
        if (unpurepak != NULL)
            com_error(ERR_DROP, "Unpure client detected." \
                      "Invalid .PK3 files referenced!\n %s", 
                      unpurepak->pak_basename);
    }

    if (fs_debug->integer > 0)
        com_printf("Can't find %s\n", filename);

    *file = 0;
    return -1;
}

/**
 * @brief 
*/
struct pack *fs_load_zip_file(char *zipfile, const char *basename)
{
    struct fileinpack *buildbuffer;
    struct pack *pack;
    unzFile uf;
    unz_global_info gi;
    unz_file_info file_info;
    char filename_inzip[MAX_ZPATH], *nameptr;
    int len, err, fs_numheaderlongs, *fs_headerlongs;
    uint32_t i, hash;

    fs_numheaderlongs = 0;

    uf = unzOpen(zipfile);
    err = unzGetGlobalInfo(uf, &gi);

    if (err != UNZ_OK)
        return NULL;

    fs_packfiles += gi.number_entry;

    len = 0;
    unzGoToFirstFile(uf);

    for (i = 0; i < gi.number_entry; i++) {
        err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip,
            sizeof(filename_inzip), NULL, 0, NULL, 0);

        if (err != UNZ_OK)
            break;

        len += strlen(filename_inzip) + 1;
        unzGoToNextFile(uf);
    }

    buildbuffer = z_malloc((gi.number_entry * sizeof(struct fileinpack)) + len);
    nameptr = ((char *) buildbuffer) + 
        gi.number_entry * sizeof(struct fileinpack);
    fs_headerlongs = z_malloc(gi.number_entry * sizeof(int));

    // ioq3
    // fs_headerlongs[fs_numheaderlongs++] = littlelong(fs_checksumfeed);

    // get the hash table size from the number of files in the zip
    // because lots of custom pk3 files have less than 32 or 64 files
    for (i = 1; i <= MAX_FILEHASH_SIZE; i <<= 1) {
        if (i > gi.number_entry)
            break;
    }

    pack = z_malloc(sizeof(struct pack) + i * sizeof(struct fileinpack *));
    pack->hashsize = i;
    pack->hashtable = (struct fileinpack **) (((char *) pack) + 
                                              sizeof(struct pack));
    for (i = 0; i < pack->hashsize; i++)
        pack->hashtable[i] = NULL;

    strncpyz(pack->pak_filename, zipfile, sizeof(pack->pak_filename));
    strncpyz(pack->pak_basename, basename, sizeof(pack->pak_basename));

    // strip .pk3 if needed
    if (strlen(pack->pak_basename) > 4 &&
        strcasecmp((pack->pak_basename + strlen(pack->pak_basename) - 4),
                  ".pk3") == 0) {
        pack->pak_basename[strlen(pack->pak_basename) - 4] = '\0';
    }

    pack->handle = uf;
    pack->numfiles = gi.number_entry;
    unzGoToFirstFile(uf);

    for (i = 0; i < gi.number_entry - 1; i++) {
        err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip,
            sizeof(filename_inzip), NULL, 0, NULL, 0);

        if (err != UNZ_OK)
            break;

        if (file_info.uncompressed_size > 0)
            fs_headerlongs[fs_numheaderlongs++] = littlelong(file_info.crc);

        strlwr(filename_inzip);
        //FS_DEBUG_PRINT("%s\n", filename_inzip);

        hash = fs_hash_filename(filename_inzip, pack->hashsize);

        buildbuffer[i].name = nameptr;
        strcpy(buildbuffer[i].name, filename_inzip);

        /*
        len2 = strlen(nameptr);
        // for (j = len2; j > 0; j--)
        do {
            a = len2;
            len2 = a - 1;

            if ((len2 < 0) || (nameptr[len2]) == '/')
                break;
        } while (nameptr[len2] != '\\');
        buildbuffer[i].a = (nameptr + a);
        */
        nameptr += strlen(filename_inzip) + 1;

        // store the file position in the zip
        unzGetCurrentFileInfoPosition(uf, &buildbuffer[i].pos);
        
        buildbuffer[i].next = pack->hashtable[hash];
        pack->hashtable[hash] = &buildbuffer[i];
        unzGoToNextFile(uf);
    }

    pack->checksum = com_block_checksum(fs_headerlongs, fs_numheaderlongs * 4);
    pack->pure_checksum = com_block_checksum_key(fs_headerlongs, 
                                                 fs_numheaderlongs * 4,
                                                 littlelong(fs_checksumfeed));

    pack->checksum = littlelong(pack->checksum);
    pack->pure_checksum = littlelong(pack->pure_checksum);

    z_free(fs_headerlongs);
    pack->buildbuffer = buildbuffer;

    return pack;
}

/**
 * @brief
 * @param qpath
 * @param buffer
 * @return
*/
int fs_read_file(const char *qpath, void **buffer)
{
    filehandle h;
    byte *buf;
    bool isconfig;
    int len;

    //if (!fs_searchpaths)
    //	com_error(ERR_FATAL, "Filesystem call made without initialization");

    if (qpath == NULL || *qpath == '\0')
        com_error(ERR_FATAL, "empty name");

    buf = NULL; // compiler warning

    isconfig = false;
    if (strstr(qpath, ".cfg") != NULL) {
        isconfig = true;

        // if this is a .cfg file and we are playing back a journal, read
        // it from the journal file
        if (com_journal != NULL && com_journal->integer == 2) {
            int r;

            com_dprintf("Loading %s from journal file.\n", qpath);
            r = fs_read(&len, sizeof(len), com_journaldatafile);
            if (r != sizeof(len)) {
                if (buffer != NULL)
                    *buffer = NULL;

                return -1;
            }

            // if the file didn't exist when the journal was created
            if (len == 0) {
                if (buffer == NULL)
                    return 1; // hack for old journal files

                *buffer = NULL;
                return -1;
            }

            if (buffer == NULL)
                return len;

            buf = hunk_allocate_temp_memory(len + 1);
            *buffer = buf;

            r = fs_read(buf, len, com_journaldatafile);
            if (r != len)
                com_error(ERR_FATAL, "read from journaldatafile failed");

            _fs_loadstack++;

            // guarantee that it will have a trailing 0 for string operations
            buf[len] = 0;
            return len;
        }
    }

    // look for it in the filesystem or packfiles
    len = fs_fopen_file_read(qpath, &h, false, 0);
    if (h == 0) {
        if (buffer != NULL)
            *buffer = NULL;

        if (isconfig && com_journal != NULL && com_journal->integer == 1) {
            com_dprintf("Writing zero for %s to journal file.\n", qpath);
            len = 0;
            fs_write(&len, sizeof(len), com_journaldatafile);
            fs_flush(com_journaldatafile);
        }

        return -1;
    }

    if (buffer == NULL) {
        if (isconfig && com_journal != NULL && com_journal->integer == 1) {
            com_dprintf("Writing len for %s to journal file.\n", qpath);
            fs_write(&len, sizeof(len), com_journaldatafile);
            fs_flush(com_journaldatafile);
        }

        fs_fclose_file(h);
        return len;
    }

    _fs_loadstack++;

    buf = hunk_allocate_temp_memory(len + 1);
    *buffer = buf;

    fs_read(buf, len, h);

    // guarantee that it will have a trailing 0 for string operations
    buf[len] = 0;

    fs_fclose_file(h);

    // if we are journalling and it is a config file, write it to the journal file
    if (isconfig && com_journal != NULL && com_journal->integer == 1) {
        com_dprintf("Writing %s to journal file.\n", qpath);

        fs_write(&len, sizeof(len), com_journaldatafile);
        fs_write(buf, len, com_journaldatafile);
        fs_flush(com_journaldatafile);
    }
    return len;
}

/**
 * @brief
 * @param buffer
 * @param len
 * @param f
 * @return
*/
int fs_read(void *buffer, int len, filehandle f)
{
    int block, remaining, read, tries;
    byte *buf;

    //if (!fs_searchpaths)
    //	com_error(ERR_FATAL, "Filesystem call made without initialization");

    if (f == 0)
        return 0;

    buf = (byte *) buffer;
    fs_readcount += len;

    if (!fsh[f].zipfile) {
        remaining = len;
        tries = 0;

        while (remaining) {
            block = remaining;
            read = fread(buf, 1, block, fsh[f].handlefiles.file.o);

            if (read == 0) {
                if (tries == 0)
                    tries = 1;
                else
                    return len - remaining; // com_error(ERR_FATAL, "fs_read: 0 bytes read");
            }

            if (read == -1)
                com_error(ERR_FATAL, "fs_read: -1 bytes read");

            remaining -= read;
            buf += read;
        }

        return len;
    } else {
        return unzReadCurrentFile(fsh[f].handlefiles.file.z, buffer, len);
    }
}

/**
 * @brief
 * @param f
*/
void fs_flush(filehandle f)
{
    fflush(fsh[f].handlefiles.file.o);
}

/**
 * @brief 
 * @param buffer 
 * @param len 
 * @param h 
 * @return 
*/
int fs_write(const void *buffer, int len, filehandle h)
{
    int remaining, tries, written, block;
    byte *buf;
    FILE *f;

    if (h == 0)
        return 0;

    f = fs_file_for_handle(h);
    buf = (byte *) buffer;

    remaining = len;
    tries = 0;
    while (remaining) {
        block = remaining;
        written = fwrite(buf, 1, block, f);
        if (written == 0) {
            if (tries == 0) {
                tries = 1;
            } else {
                com_printf("fs_write: 0 bytes written\n");
                return 0;
            }
        }

        if (written == -1) {
            com_printf("fs_write: -1 bytes written\n");
            return 0;
        }

        remaining -= written;
        buf += written;
    }

    if (fsh[h].handlesync)
        fflush(f);

    return len;
}

void fs_printf(filehandle h, const char *fmt, ...)
{
    va_list argptr;
    char msg[MAX_PRINT_MSG];

    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    va_end(argptr);

    fs_write(msg, strlen(msg), h);
}

void fs_free_file(void *buf)
{
    if (buf == NULL)
        com_error(ERR_FATAL, "fs_free_file(null)");

    _fs_loadstack--;
    hunk_free_temp_memory(buf);
}
