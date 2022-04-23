#include <assert.h>

#include "fs_local.h"
#include "strings/stringlib.h"

void fs_replace_separators(char *path)
{
    bool gotsep = false;

    for (char *s = path; *s != '\0'; s++) {
        if (*s == '/' || *s == '\\') {
            if (!gotsep) {
                *s = PATH_SEP;
                gotsep = true;
            } else {
                memmove(s, s + 1, strnlen(s, MAX_PATH));
            }
        } else {
            gotsep = false;
        }
    }
}

void test_fs_replace_separators(void)
{
    char path[MAX_PATH];

    #ifdef PLATFORM_WINDOWS
    strncpy(path, "test/1234/5678", sizeof(path));
    #else
    strncpy(path, "test\\1234\\5678", sizeof(path));
    #endif

    fs_replace_separators(path);

    #ifdef PLATFORM_WINDOWS
    assert(strncmp(path, "test\\1234\\5678", sizeof(path)) == 0);
    #else
    assert(strncmp(path, "test/1234/5678", sizeof(path)) == 0);
    #endif

    printf("%s succeeded\n", __func__);
}
