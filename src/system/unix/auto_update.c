#include "shared.h"
#include "common.h"

#include "common/memory.h"
#include "common/print.h"
#include "strings/stringlib.h"

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include "unzip.h"

#if defined USE_CURL && !defined DISABLE_AUTO_UPDATE
#include <curl/curl.h>

#define UPDATE_URL	"https://update.iocod.org"

enum auto_update_error {
    AU_OK,
    AU_CURL_ERROR,
    AU_REQUEST_FAILED,
    AU_COULD_NOT_REQUEST,
    AU_NO_UPDATE,
    AU_DOWNLOAD_FAILED,
    AU_NO_FILE_SENT,
    AU_TEMP_DIR,
    AU_UNZ_OPEN,
    AU_UNZ_GLOBAL_INFO,
    AU_UNZ_FILE_INFO,
    AU_UNZ_OPEN_FILE,
    AU_UNZ_OPEN_FILE_WRITE,
    AU_UNZ_FILE_EXTRACT,
    AU_UNZ_NEXT_FILE,
    AU_STAT_FAILED,
    AU_CHMOD_FAILED,
    AU_RENAME_FAILED,
    AU_MISSING_ARGV
};

static void update_printf(const char *fmt, ...)
{
    va_list argptr;
    char msg[MAX_PRINT_MSG];

    va_start(argptr, fmt);
    vsnprintf(msg, sizeof(msg), fmt, argptr);
    msg[strlen(msg)] = '\0';
    va_end(argptr);

    com_printf("[auto_update] %s", msg);
}

static char *remote_version;
static size_t check_header_for_version_string(char *str, size_t size, 
                                       size_t num_items, UNUSED void *user_data)
{
    int len;
    size_t numbytes = size * num_items;

    if (strstr(str, "location: ") != NULL) {
        len = strlen(str + 10) - 2;

        remote_version = z_malloc(len);
        strncpy(remote_version, str + 10, len);
    }
    
    return numbytes;
}

static bool download_failed = false;
static char *download;
static size_t check_for_failed_download(char *str, size_t size,
                                        size_t num_items, UNUSED void *user_data)
{
    int len;
    size_t numbytes = size * num_items;

    if (strstr(str, "location: ") != NULL && !download_failed) {
        len = strlen(str + 10) - 2;

        if (strstr(str, "download-failed") != NULL) {
            download_failed = true;
        } else {
            download = z_malloc(len);
            strncpy(download, str + 10, len);
        }
    }

    return numbytes;
}

struct fileptr {
    char *filename;
    FILE *stream;
};
static struct fileptr file;

static size_t write_file_data(void *buffer, size_t size, size_t num_items,
                              void *stream)
{
    struct fileptr *out = (struct fileptr *) stream;

    if (out != NULL && out->stream == NULL) {
        out->stream = fopen(out->filename, "wb");
        if (out->stream == NULL)
            return -1;
    }

    return fwrite(buffer, size, num_items, out->stream);
}

static char *get_random_filename(const char *ext)
{
    char buf[MAX_OSPATH];
    int ret, tries = 0;
    struct stat statbuf;

    srand(time(NULL) * getpid());

    while (tries < 3) {
        if (ext != NULL)
            ret = snprintf(buf, MAX_OSPATH - 1, "tmp_%d.%s", rand(), ext);
        else
            ret = snprintf(buf, MAX_OSPATH - 1, "tmp_%d", rand());

        if (ret == -1)
            return NULL;

        if (stat(buf, &statbuf) == -1)
            return strdup(buf);

        tries++;
    }

    return NULL;
}

static int download_update(void)
{
    CURL *handle;
    CURLcode res;

    // GET request
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, UPDATE_URL);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, PLATFORM_STRING);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, check_for_failed_download);

    res = curl_easy_perform(handle);
    curl_easy_cleanup(handle);

    if (res != CURLE_OK) {
        update_printf("cURL failed: %s\n", curl_easy_strerror(res));
        return AU_CURL_ERROR;
    }

    // it's possible that the new release does not have our architecture yet
    if (download_failed) {
        update_printf("Download failed\n");
        return AU_DOWNLOAD_FAILED;
    }

    // server error
    if (download == NULL) {
        update_printf("Server didn't send a file\n");
        return AU_NO_FILE_SENT;
    }

    update_printf("Got file %s\n", download);

    file.filename = get_random_filename("zip");
    file.stream = NULL;

    // grab file from github
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, download);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_file_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &file);

    res = curl_easy_perform(handle);
    curl_easy_cleanup(handle);

    if (file.stream != NULL)
        fclose(file.stream);

    if (res != CURLE_OK) {
        update_printf("cURL failed: %s\n", curl_easy_strerror(res));
        return AU_CURL_ERROR;
    }

    update_printf("Wrote file to %s\n", file.filename);

    return AU_OK;
}

static char *dir;
static char *game_dll_name;
static int unpack_update(void)
{
    int ret = AU_OK;
    uint32_t i;
    char filename[MAX_QPATH], tmp[MAX_QPATH], outname[MAX_OSPATH];
    unzFile *zip;
    unz_global_info gi;
    unz_file_info fi;
    int flen, err;
    FILE *out;

    #define BUFSZ 32768
    char buffer[BUFSZ];

    dir = get_random_filename(NULL);
    update_printf("Creating temp dir %s\n", dir);
    if (mkdir(dir, S_IRWXU) == -1) {
        update_printf("Error making temp dir: %s\n", strerror(errno));
        ret = AU_TEMP_DIR;
        goto end;
    }

    snprintf(tmp, sizeof(tmp), "%s/%s", dir, file.filename);
    rename(file.filename, tmp);

    update_printf("Opening %s\n", tmp);

    zip = unzOpen(tmp);
    if (zip == NULL) {
        update_printf("Couldn't open %s\n", tmp);
        ret = AU_UNZ_OPEN;
        goto end;
    }

    if (unzGetGlobalInfo(zip, &gi) != UNZ_OK) {
        update_printf("Couldn't read the global info in file %s\n", 
                      tmp);
        ret = AU_UNZ_GLOBAL_INFO;
        goto end;
    }

    unzGoToFirstFile(zip);

    // extract all files
    for (i = 0; i < gi.number_entry; i++) {
        if (unzGetCurrentFileInfo(zip, &fi, filename, sizeof(filename),
                                  NULL, 0, NULL, 0) != UNZ_OK) {
            update_printf("Couldn't read file info in %s\n", tmp);
            ret = AU_UNZ_FILE_INFO;
            goto end;
        }

        flen = strlen(filename);
        if (filename[flen - 1] == '/' || filename[flen - 1] == '\\') {
            update_printf("New dir: %s\n", filename);
            mkdir(filename, S_IRWXU);
        } else {
            update_printf("Extract: %s\n", filename);
            if (unzOpenCurrentFile(zip) != UNZ_OK) {
                update_printf("Couldn't open file %s\n", filename);
                ret = AU_UNZ_OPEN_FILE;
                goto end;
            }

            if (strstr(filename, "game.mp.") != NULL ||
                strstr(filename, "game_mp_") != NULL)
                game_dll_name = strdup(filename);

            com_memset(outname, 0, sizeof(outname));
            snprintf(outname, sizeof(outname), "%s/%s", dir, filename);
            out = fopen(outname, "wb");
            if (out == NULL) {
                update_printf("Couldn't open file for writing %s", outname);
                unzCloseCurrentFile(zip);
                ret = AU_UNZ_OPEN_FILE_WRITE;
                goto end;
            }

            err = UNZ_OK;
            do {
                err = unzReadCurrentFile(zip, buffer, BUFSZ);
                if (err < 0) {
                    update_printf("Error: %d\n", err);
                    unzCloseCurrentFile(zip);
                    err = AU_UNZ_FILE_EXTRACT;
                    goto end;
                }

                if (err > 0)
                    fwrite(buffer, err, 1, out);
            } while (err > 0);

            fclose(out);
        }

        unzCloseCurrentFile(zip);

        if ((i + 1) < gi.number_entry) {
            if (unzGoToNextFile(zip) != UNZ_OK) {
                update_printf("Could not read next file\n");
                err = AU_UNZ_NEXT_FILE;
                goto end;
            }
        }
    }

end:
    unzClose(zip);
    unlink(tmp); // remove temp zip
    return ret;
}

static int exe_path(char *buffer, size_t *size)
{
    ssize_t n;

    if (buffer == NULL || size == NULL || *size == 0)
        return -1;

    n = *size - 1;
    if (n > 0)
        n = readlink("/proc/self/exe", buffer, n);

    if (n == -1)
        return -1;

    buffer[n] = '\0';
    *size = n;

    return 0;
}

static int apply_update(char *argv[])
{
    size_t path_len = MAX_OSPATH;
    char tmp[MAX_OSPATH], out[MAX_OSPATH];
    char *path = z_malloc(path_len);
    struct stat current_st;
    int res, ret = AU_OK;

    if (game_dll_name != NULL) {
        snprintf(tmp, sizeof(tmp), "%s/%s", dir, game_dll_name);
        snprintf(out, sizeof(tmp), "main/%s", game_dll_name);

        res = stat(out, &current_st);
        if (res != 0) {
            update_printf("stat failed for %s\n", out);
            return AU_STAT_FAILED;
        }

        res = chmod(tmp, current_st.st_mode | S_IXUSR);
        if (res != 0) {
            update_printf("chmod failed for %s\n", tmp);
            return AU_CHMOD_FAILED;
        }

        res = rename(tmp, out);
        if (res != 0) {
            update_printf("Failed to rename %s to %s\n", tmp, out);
            return AU_RENAME_FAILED;
        }
    }

    if (exe_path(path, &path_len) != 0) {
        if (argv[0] == NULL) {
            update_printf("Missing argv[0]\n");
            ret = AU_MISSING_ARGV;
            goto end;
        }

        com_memcpy(path, argv[0], strlen(argv[0]));
    }

    res = stat(path, &current_st);
    if (res != 0) {
        update_printf("stat failed for %s\n", path);
        ret = AU_STAT_FAILED;
        goto end;
    }

    snprintf(tmp, sizeof(tmp), "%s/%s", dir, "iocod");
    snprintf(out, sizeof(out), "%s", "iocod");

    res = chmod(tmp, current_st.st_mode | S_IXUSR);
    if (res != 0) {
        update_printf("chmod failed for %s\n", path);
        ret = AU_CHMOD_FAILED;
        goto end;
    }

    res = rename(tmp, out);
    if (res != 0) {
        update_printf("Failed to rename %s to %s\n", tmp, out);
        ret = AU_RENAME_FAILED;
        goto end;
    }

    // delete dir finally
    rmdir(dir);
    z_free(dir);

    update_printf("Restarting...\n");
    res = execv(path, argv);

end:
    z_free(path);
    return ret;
}

int auto_update(UNUSED int argc, char *argv[])
{
    CURL *handle;
    CURLcode res;
    int ret = AU_OK;
    char current_version[32] = {0};

    strcat(current_version, "v");
    strcat(current_version, VERSION_NUMBER);

    update_printf("Checking for updates...\n");

    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, UPDATE_URL);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, PLATFORM_STRING);
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, 
                     check_header_for_version_string);

    res = curl_easy_perform(handle);
    curl_easy_cleanup(handle);

    if (res != CURLE_OK) {
        update_printf("cURL failed: %s\n", curl_easy_strerror(res));
        ret = AU_CURL_ERROR;
        goto end;
    }

    if (remote_version == NULL) {
        update_printf("Could not request remote version\n");
        ret = AU_COULD_NOT_REQUEST;
        goto end;
    }

    // request-failed tells us either
    // 1. github had an error and couldn't give us a list of releases
    // 2. or there was an error decoding the json data (unlikely)
    if (strncmp(remote_version, "request-failed", 15) == 0) {
        update_printf("Remote version request failed\n");
        ret = AU_REQUEST_FAILED;
        goto end;
    }

    // check against our version
    if (strcmp(current_version, remote_version) == 0) {
        update_printf("No update required\n");
        ret = AU_NO_UPDATE;
        goto end;
    }

    update_printf("Newer version detected: %s (we have %s)\n",
                  remote_version, current_version);

    // download the new update
    ret = download_update();
    if (ret != AU_OK)
        goto end;

    // unpack and extract 
    ret = unpack_update();
    if (ret != AU_OK)
        goto end;

    // apply update
    ret = apply_update(argv);
    if (ret != AU_OK)
        goto end;

end:
    // clean up
    z_free(remote_version);
    z_free(download);
    z_free(file.filename);
    z_free(dir);
    return ret;
}

#else /* defined USE_CURL && !defined DISABLE_AUTO_UPDATE */
int auto_update(UNUSED int argc, UNUSED char *argv[])
{
    return 0;
}
#endif
