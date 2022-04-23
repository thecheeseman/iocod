#include "fs_local.h"
#include "strings/stringlib.h"

void test_suite(void)
{
    test_fs_replace_separators();
    test_fs_is_valid_path();
    test_fs_create_path();
    test_fs_build_path();
    test_fs_file_list();
}

int main(int argc, char *argv[])
{
    test_suite();

    fs_init();
    return 0;
}
