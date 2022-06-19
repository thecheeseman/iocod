#include "ic_test.h"

// disable warning: null argument where non-null required [-Wnonnull]
#if defined IC_PLATFORM_GCC || defined IC_PLATFORM_CLANG
IC_DIAGNOSTIC_PUSH
IC_SILENCE_WARNING(-Wnonnull)
#endif

#define TEST _ic_strupr

int TEST_MAIN()
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

#if defined IC_PLATFORM_GCC || defined IC_PLATFORM_CLANG
IC_DIAGNOSTIC_POP
#endif
