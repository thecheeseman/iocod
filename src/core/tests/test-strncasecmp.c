#include "ic_test.h"

#define TEST _ic_strncasecmp

int TEST_MAIN()
{
    // OK to compare NULL against NULL
    IC_ASSERT(TEST(NULL, NULL, 0) == 0);

    // anything with 0 size is the same
    IC_ASSERT(TEST("string1", "STRING1", 0) == 0);

    // any non-NULL value should be > NULL 
    IC_ASSERT(TEST(NULL, "", 1) == -1);
    IC_ASSERT(TEST("", NULL, 1) == 1);

    // blank
    IC_ASSERT(TEST("", "", 10) == 0);

    // standard
    IC_ASSERT(TEST("12345678", "1234", 8) > 0);
    IC_ASSERT(TEST("1234", "12345678", 8) < 0);
    IC_ASSERT(TEST("STRING", "string", 6) == 0);
    IC_ASSERT(TEST("lOngeR STRING", "longer string", 20) == 0);

    return 0;
}
