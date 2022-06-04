#include "ic_test.h"

int TEST_MAIN()
{
    char path[256];

    strncpyz(path, "test/path/goes/here", sizeof(path));
    ic_default_extension(path, sizeof(path), ".txt");
    IC_ASSERT(strcmp(path, "test/path/goes/here.txt") == 0);

    strncpyz(path, "another/test/path/with.ext", sizeof(path));
    ic_default_extension(path, sizeof(path), ".dat");
    IC_ASSERT(strcmp(path, "another/test/path/with.ext") == 0);

    return 0;
}
