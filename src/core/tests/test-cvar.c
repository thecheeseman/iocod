#include "ic_test.h"

struct cvar *com_sv_running;

int TEST_MAIN()
{
    cv_init();
    com_sv_running = cv_get("sv_running", "1", CV_ROM);

    /* cv_get_xxx */
    cv_get("test", "1234", CV_NONE);
    cv_get("anotherone", "1", CV_USER_INFO);
    cv_get("pi", "3.14", CV_NONE);

    IC_ASSERT(strcmp(cv_get_string("test"), "1234") == 0);
    IC_ASSERT(cv_get_integer("anotherone") == 1);
    IC_ASSERT((cv_int)(cv_get_value("pi") * 100) == 314);

    /* cv_validate_string */
    IC_ASSERT(cv_validate_string("this is a valid string") == true);
    IC_ASSERT(cv_validate_string("this is an \"invalid\" string") == false);
    IC_ASSERT(cv_validate_string("this is an \\invalid \\string") == false);
    IC_ASSERT(cv_validate_string("this is an ;invalid; string") == false);

    /* cv_clear_foreign_chars */
    char *str1 = "this €is a string with foreignŒ charsŸŸŸŸ";
    char *str2 = "this is a string with foreign chars"; /* cleared version */
    char *str3 = cv_clear_foreign_chars(str1);
    IC_ASSERT(strcmp(str2, str3) == 0);

    /* cv_get_string_buffer */
    char *bufstr = "this is a longer string designed to be tested with cv_get_string_buffer";
    cv_get("buf", bufstr, CV_NONE);

    char buf[256];
    cv_get_string_buffer("buf", buf, sizeof(buf));
    IC_ASSERT(strcmp(bufstr, buf) == 0);

    /* cv_set_xxx */
    cv_set_string("str", "this is a string");
    IC_ASSERT(strcmp("this is a string", cv_get_string("str")) == 0);

    cv_set_value("value", 9.20);
    IC_ASSERT((cv_int) (cv_get_value("value") * 10) == 92);

    cv_set_integer("integer", 42);
    IC_ASSERT(cv_get_integer("integer") == 42);

    cv_get("str", "test string", CV_LATCH);
    struct cvar *v = cv_set_string_latched("str", "latched version");
    IC_ASSERT(v != NULL);
    IC_ASSERT(v->latched_string != NULL);
    IC_ASSERT(strcmp(v->latched_string, "latched version") == 0);

    /* cv_reset */
    cv_get("resettest", "reset string test", CV_NONE);
    cv_set_string("resettest", "some other value");

    v = cv_reset("resettest");
    IC_ASSERT(v != NULL);
    IC_ASSERT(v->string != NULL);
    IC_ASSERT(strcmp(v->string, "reset string test") == 0);

    /* cv_write_defaults */
    ic_printf("cv_write_defaults:\n");
    cv_write_defaults(0);

    /* cv_write_variables */
    ic_printf("cv_write_variables:\n");
    cv_write_variables(0);

    cv_shutdown();
    return 0;
}
