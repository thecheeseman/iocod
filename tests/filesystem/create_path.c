#include "fs_local.h"

#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef PLATFORM_WINDOWS
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include "strings/stringlib.h"

static bool directory_exists(const char *path)
{
    if (access(path, 0) == 0) {
        struct stat status;
        stat(path, &status);

        return (status.st_mode & S_IFDIR) != 0;
    }

    return false;
}

#ifdef PLATFORM_WINDOWS
#define MKDIR(p) (void) _mkdir(p)
#else
#define MKDIR(p) mkdir(p, 0777)
#endif

// returns: false if failed, true if succeed
bool fs_create_path(const char *cpath)
{
    if (!fs_is_valid_path(cpath))
        return false;

    char path[MAX_PATH];
    strncpyz(path, cpath, sizeof(path));

    for (char *offset = path + 1; *offset != '\0'; offset++) {
        if (*offset == '/' || *offset == '\\') {
            *offset = '\0';

            // skip trying to build paths that start with .
            if (strnlen(path, MAX_PATH) == 1 && path[0] == '.') {
                *offset = PATH_SEP;
                continue;
            }

            if (!directory_exists(path))
                MKDIR(path);

            *offset = PATH_SEP;
        }
    }

    return true;
}

void test_fs_create_path(void)
{
    assert(fs_create_path(NULL) == false);
    assert(fs_create_path("") == false);

    // disallow '/' or '\' at beginning
    assert(fs_create_path("/home/test/test.txt") == false);
    assert(fs_create_path("\\home\\test\\test.txt") == false);

    // disallow UNC paths on windows
    assert(fs_create_path("\\\\server\\test.txt") == false);
    assert(fs_create_path("c:\\program files\\test.txt") == false);
    assert(fs_create_path("c:program files\\test.txt") == false);

    // OK
    assert(fs_create_path(".") == true);
    assert(fs_create_path(".\\test.txt") == true);
    assert(fs_create_path("./test.txt") == true);
    assert(fs_create_path("test/test2/test3") == true);
    assert(fs_create_path("maps/mp/gametypes/test.txt") == true);

    // finally some real world tests sourced from the internet
    assert(fs_create_path("D:\\Data\\Final\\Infrastructure.gdb\\EastValley\\powerlines") == false);
    assert(fs_create_path("E:\\data\\telluride\\newdata.gdb\\slopes") == false);
    assert(fs_create_path("..\\..\\.\\Final\\..\\Shapefiles\\.\\Landuse") == false);
    assert(fs_create_path("\\\\pondermatic\\public\\studyarea.gdb") == false);
    assert(fs_create_path("\\\\omnipotent\\shared_stuff\\wednesday\\tools") == false);

    printf("%s success\n", __func__);
}
