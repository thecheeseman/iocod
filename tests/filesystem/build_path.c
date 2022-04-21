#include <assert.h>

#include "fs_local.h"
#include "strings/stringlib.h"

void fs_build_path(const char *base, const char *game,
                   const char *path, char *buffer) 
{
    if (buffer == NULL)
        return;

    if (base == NULL || path == NULL) {
        *buffer = '\0';
        return;
    }

    if (game == NULL || *game == '\0')
        game = fs.game_dir;

    size_t base_len = strnlen(base, MAX_PATH);
    size_t game_len = strnlen(game, MAX_PATH);
    size_t path_len = strnlen(path, MAX_PATH);

    if ((base_len + game_len + path_len) > MAX_PATH)
        com_error(ERR_FATAL, "path length exceeded");

    memcpy(buffer, base, base_len);
    buffer[base_len] = '/';
    memcpy(buffer + base_len + 1, game, game_len);
    buffer[base_len + game_len + 1] = '/';
    memcpy(buffer + game_len + base_len + 2, path, path_len + 1);

    fs_replace_separators(buffer);
}

void test_fs_build_path(void)
{
    char path[MAX_PATH];

    strncpy(fs.game_dir, "main", sizeof("main"));

    fs_build_path(NULL, NULL, NULL, path);
    assert(path[0] == '\0');

    fs_build_path("test", "main", "test.pk3", path);
    #ifdef PLATFORM_WINDOWS
    assert(strncmp(path, "test\\main\\test.pk3", sizeof(path)) == 0);
    #else
    assert(strncmp(path, "test/main/test.pk3", sizeof(path)) == 0);
    #endif

    fs_build_path("test", NULL, "test2.pk3", path);
    #ifdef PLATFORM_WINDOWS
    assert(strncmp(path, "test\\main\\test2.pk3", sizeof(path)) == 0);
    #else
    assert(strncmp(path, "test/main/test2.pk3", sizeof(path)) == 0);
    #endif

    printf("%s success\n", __func__);
}
