#include "../iocod.h"
#include <stdio.h>

#define TEST _ic_strlwr

int main(void)
{
    IC_ASSERT(TEST(NULL) == NULL);
    IC_ASSERT(TEST("") != NULL);

    char *t1 = TEST("TEST");
    IC_ASSERT(strcmp(t1, "test") == 0);

    char *t2 = TEST("tHiS string is !@#$ wacky");
    IC_ASSERT(strcmp(t2, "this string is !@#$ wacky") == 0);

    char *t3 = TEST("1234");
    IC_ASSERT(strcmp(t3, "1234") == 0);

    return 0;
}
