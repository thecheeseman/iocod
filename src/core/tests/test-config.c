#include "ic_test.h"

#define CFG_HEADER(x) { .name = "hdr", .type = CFG_HEADER, .default_str = x }
#define CFG_COMMENT(x) { .name = "cmt", .type = CFG_COMMENT, .default_str = x }
#define CFG_BLANK() { .name = "blk", .type = CFG_BLANK }
#define CFG_BOOL(n, v) { .name = n, .type = CFG_BOOL, .default_str = #v }
#define CFG_INT(n, v) { .name = n, .type = CFG_INT, .default_str = #v }
#define CFG_FLOAT(n, v) { .name = n, .type = CFG_FLOAT, .default_str = #v }
#define CFG_STR(n, v) { .name = n, .type = CFG_STRING, .default_str = v }
#define CFG_END() { .name = "", .type = CFG_END }

struct configopt opts[] = {
    CFG_HEADER("test header"),
    CFG_COMMENT("this is a test comment"),
    CFG_BOOL("test_bool", YES),
    CFG_INT("int", 42),
    CFG_FLOAT("pi", 3.14),
    CFG_STR("str", "this is a string value"),

    CFG_END()
};

int TEST_MAIN()
{
    struct config *cfg = config_init("test.conf", opts);

    config_shutdown();
    return 0;
}
