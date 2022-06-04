#include "ic_test.h"

int TEST_MAIN()
{
    char info[INFO_STRING_BIG] = {0};

    ifbig_set_value_for_key(info, "test1", "value1");
    ifbig_set_value_for_key(info, "test2", "value2");
    ifbig_set_value_for_key(info, "another test", "another value");

    char *info_test = "\\test1\\value1\\test2\\value2\\another test\\another value";
    IC_ASSERT(strcmp(info, info_test) == 0);

    ifbig_remove_key(info, "test2");
    info_test = "\\test1\\value1\\another test\\another value";
    IC_ASSERT(strcmp(info, info_test) == 0);

    IC_ASSERT(ifbig_set_value_for_key(info, "bad;string", "bad\\value") == false);
    IC_ASSERT(ifbig_remove_key(info, "bad;string") == false);

    IC_ASSERT(if_remove_key(info, "another test") == true);

    return 0;
}
