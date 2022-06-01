#include "../iocod.h"
#include <stdio.h>

int main(void)
{   
    int len;
    char test[64];

    // cannot write to NULL
    IC_ASSERT(snprintfz(NULL, sizeof(test), "%s", "asdf") == -1);

    // cannot write 0 bytes
    IC_ASSERT(snprintfz(test, 0, "%s", "asdf") == -1);

    // cannot write NULL fmt specifier
    IC_ASSERT(snprintfz(test, sizeof(test), NULL) == -1);

    // normal usage case
    char buf[16];
    len = snprintfz(buf, sizeof(buf), "this is a %s\n", "test");
    IC_ASSERT_MSG(len != -1, "failed writing string");
    IC_ASSERT_MSG(strncmp(buf, "this is a test\n", sizeof(buf)) == 0, 
                  "strings do not match");

    // buffer that is too small, clipping occurs
    char toosmall[4];
    len = snprintfz(toosmall, sizeof(toosmall), "another test");
    IC_ASSERT_MSG(len != -1, "failed to write string");
    IC_ASSERT_MSG(len == 12, "length does not report correctly");
    IC_ASSERT_MSG(strncmp(toosmall, "ano", sizeof(toosmall)) == 0,
                  "strings do not match");

    return 0;
}
