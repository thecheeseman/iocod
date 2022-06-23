#include "ic_test.h"
#include <math.h>

struct confopt opts[] = {
    CONF_HEADER("test header"),
    CONF_COMMENT("this is a test comment"),
    CONF_BOOL("test_bool", on),
    CONF_INT("int", 42),
    CONF_FLOAT("pi", 3.14),
    CONF_STRING("str", "this is a string value"),

    CONF_END()
};

int TEST_MAIN()
{
    struct conf *cfg = conf_init("test.conf", opts, 0);

    // test getting values
    bool test_bool = conf_get_bool(cfg, "test_bool");
    IC_ASSERT(test_bool);

    conf_int i = conf_get_int(cfg, "int");
    IC_ASSERT(i == 42);

    conf_float pi = conf_get_float(cfg, "pi");
    IC_ASSERT(fabs(pi - 3.14) < 0.0001); // float comparisons

    char *str = conf_get_string(cfg, "str");
    IC_ASSERT(strcmp(str, "this is a string value") == 0);

    // test setting values
    IC_ASSERT(conf_set_bool(cfg, "test_bool", false));
    IC_ASSERT(conf_get_bool(cfg, "test_bool") == false);

    IC_ASSERT(conf_set_int(cfg, "int", 99));
    IC_ASSERT(conf_get_int(cfg, "int") == 99);

    IC_ASSERT(conf_set_float(cfg, "pi", 3.0));
    IC_ASSERT(fabs(conf_get_float(cfg, "pi") - 3.0) < 0.00001);

    IC_ASSERT(conf_set_string(cfg, "str", "another test string"));
    IC_ASSERT(strcmp(conf_get_string(cfg, "str"), "another test string") == 0);

    conf_shutdown(cfg);

    config_init();
    cfg = config_get();
    IC_ASSERT(cfg != NULL);
    conf_dump_options(cfg);
    config_shutdown();

    return 0;
}
