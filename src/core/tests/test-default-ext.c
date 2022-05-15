#include "iocod.h"

int main(void)
{
    char path[256];

    strncpyz(path, "test/path/goes/here", sizeof(path));
    default_extension(path, sizeof(path), ".txt");
    IC_ASSERT(strcmp(path, "test/path/goes/here.txt") == 0);

    strncpyz(path, "another/test/path/with.ext", sizeof(path));
    default_extension(path, sizeof(path), ".dat");
    IC_ASSERT(strcmp(path, "another/test/path/with.ext") == 0);

    return 0;
}
