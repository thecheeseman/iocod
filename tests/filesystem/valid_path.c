#include <assert.h>
#include <ctype.h>
#include "strings/stringlib.h"
#include "fs_local.h"

/*
    These are all the valid file paths on Windows:
        abs C:\temp\test-folder\test-file.txt
        abs \temp\test-folder\test-file.txt
        rel test-folder\test-file.txt
        rel .\test-file.txt
        rel ..\test-folder\test-file.txt
        rel C:test-folder\test-file.txt

        abs \\127.0.0.1\c$\temp\test-folder\test-file.txt
        abs \\LOCALHOST\\c$\temp\test-folder\test-file.txt
        abs \\.\c:\temp\test-folder\test-file.txt
        abs \\?\c:\temp\test-folder\test-file.txt
        abs \\.\Volume{b75e2c83-0000-0000-0000-602f00000000}\temp\test-folder\test-file.txt
        abs \\?\Volume{b75e2c83-0000-0000-0000-602f00000000}\temp\test-folder\test-file.txt

    These are all the valid file paths on Unix:
        abs /home/test/test-file.txt
        rel test/test-file.txt
        rel ./test-file.txt
        rel ../test/test-file

    The ONLY valid paths we want are _relative_ to the base game directory,
    therefore we must invalidate the following:
            C:\ C:
            \\ /
            ..
*/
bool fs_is_valid_path(const char *path)
{
    if (path == NULL || *path == '\0')
        return false;

    size_t len = strnlen(path, MAX_PATH);

    // no paths that start with "/" or "\"
    if (len > 1 && (path[0] == '/' || path[0] == '\\'))
        return false;

    // Windows "C:" and "C:\"
    if (len > 2 && (isalpha(path[0]) && path[1] == ':'))
        return false;

    // relative paths on any system
    if (strstr(path, "..") || strstr(path, "::"))
        return false;

    return true;
}

void test_fs_is_valid_path(void)
{
    assert(fs_is_valid_path(NULL) == false);
    assert(fs_is_valid_path("") == false);

    // disallow '/' or '\' at beginning
    assert(fs_is_valid_path("/home/test/test.txt") == false);
    assert(fs_is_valid_path("\\home\\test\\test.txt") == false);

    // disallow UNC paths on windows
    assert(fs_is_valid_path("\\\\server\\test.txt") == false);
    assert(fs_is_valid_path("c:\\program files\\test.txt") == false);
    assert(fs_is_valid_path("c:program files\\test.txt") == false);

    // OK
    assert(fs_is_valid_path(".") == true);
    assert(fs_is_valid_path(".\\test.txt") == true);
    assert(fs_is_valid_path("./test.txt") == true);

    // finally some real world tests sourced from the internet
    assert(fs_is_valid_path("D:\\Data\\Final\\Infrastructure.gdb\\EastValley\\powerlines") == false);
    assert(fs_is_valid_path("E:\\data\\telluride\\newdata.gdb\\slopes") == false);
    assert(fs_is_valid_path("..\\..\\.\\Final\\..\\Shapefiles\\.\\Landuse") == false);
    assert(fs_is_valid_path("\\\\pondermatic\\public\\studyarea.gdb") == false);
    assert(fs_is_valid_path("\\\\omnipotent\\shared_stuff\\wednesday\\tools") == false);

    printf("%s success\n", __func__);
}
