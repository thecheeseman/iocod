#include "ic_test.h"

int TEST_MAIN()
{
    char test[256];

    // NULL
    IC_ASSERT(ic_append_extension(NULL, 0, NULL) == false);
    IC_ASSERT(ic_append_extension("asdf", 0, NULL) == false);
    IC_ASSERT(ic_append_extension("asdf", 120, NULL) == false);

    // append
    snprintf(test, sizeof(test), "test-path");
    IC_ASSERT(ic_append_extension(test, sizeof(test), ".ext"));
    IC_ASSERT(strcmp(test, "test-path.ext") == 0);

    // don't append to already existing extensions
    snprintf(test, sizeof(test), "test-path.ext");
    IC_ASSERT(ic_append_extension(test, sizeof(test), ".ext"));
    IC_ASSERT(strcmp(test, "test-path.ext") == 0);

    // append a new extension if one already exists
    snprintf(test, sizeof(test), "test-path.dll");
    IC_ASSERT(ic_append_extension(test, sizeof(test), ".ext"));
    IC_ASSERT(strcmp(test, "test-path.dll.ext") == 0);

    // path test
    snprintf(test, sizeof(test), "test/path");
    IC_ASSERT(ic_append_extension(test, sizeof(test), ".ext"));
    IC_ASSERT(strcmp(test, "test/path.ext") == 0);

    // do not append extensions to paths ending with '/'
    snprintf(test, sizeof(test), "testpath/");
    IC_ASSERT(ic_append_extension(test, sizeof(test), ".ext"));
    IC_ASSERT(strcmp(test, "testpath/") == 0);

    return 0;
}
