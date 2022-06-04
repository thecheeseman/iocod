#include "ic_test.h"

int TEST_MAIN()
{
    char *t1 = va("this is a %s", "test");
    IC_ASSERT(strcmp(t1, "this is a test") == 0);

    char max[16384];
    for (size_t i = 0; i < sizeof(max); i++)
        max[i] = 'a';

    char *test = va("%s", max);
    IC_ASSERT(strlen(test) == 8191);

    return 0;
}
