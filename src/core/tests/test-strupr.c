#include "../iocod.h"
#include <stdio.h>

#define TEST _ic_strupr

int main(void)
{
    IC_ASSERT(TEST(NULL) == NULL);
    IC_ASSERT(TEST("") != NULL);

    char *t1 = TEST("test");
    IC_ASSERT(strcmp(t1, "TEST") == 0);

    char *t2 = TEST("tHiS string is !@#$ wacky");
    IC_ASSERT(strcmp(t2, "THIS STRING IS !@#$ WACKY") == 0);

    char *t3 = TEST("1234");
    IC_ASSERT(strcmp(t3, "1234") == 0);

    return 0;
}
