#include "ic_test.h"

// disable warning: null argument where non-null required [-Wnonnull]
#if defined IC_PLATFORM_GCC || defined IC_PLATFORM_CLANG
IC_DIAGNOSTIC_PUSH
IC_SILENCE_WARNING(-Wnonnull)
#endif

#define TEST _ic_strcasecmp

int TEST_MAIN()
{
    // OK to compare NULL against NULL
    IC_ASSERT(TEST(NULL, NULL) == 0);

    // any non-NULL value should be > NULL 
    IC_ASSERT(TEST(NULL, "") == -1);
    IC_ASSERT(TEST("", NULL) == 1);

    // blank
    IC_ASSERT(TEST("", "") == 0);

    // standard
    IC_ASSERT(TEST("12345678", "1234") > 0);
    IC_ASSERT(TEST("1234", "12345678") < 0);
    IC_ASSERT(TEST("STRING", "string") == 0);
    IC_ASSERT(TEST("lOngeR STRING", "longer string") == 0);

    return 0;
}

#if defined IC_PLATFORM_GCC || defined IC_PLATFORM_CLANG
IC_DIAGNOSTIC_POP
#endif
