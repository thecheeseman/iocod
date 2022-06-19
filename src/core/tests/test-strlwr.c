#include "ic_test.h"

// disable warning: null argument where non-null required [-Wnonnull]
#if defined IC_PLATFORM_GCC || defined IC_PLATFORM_CLANG
IC_DIAGNOSTIC_PUSH
IC_SILENCE_WARNING(-Wnonnull)
#endif

#define TEST _ic_strlwr

int TEST_MAIN()
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

#if defined IC_PLATFORM_GCC || defined IC_PLATFORM_CLANG
IC_DIAGNOSTIC_POP
#endif
