#include "iocod.h"

#define MAX_PARSE_INFO 16
static struct parse_info info[MAX_PARSE_INFO];
static size_t info_num = 0;
struct parse_info *pi = &info[0];

IC_PUBLIC
IC_NON_NULL(1)
void ps_begin_session(const char *name)
{
    if (info_num == MAX_PARSE_INFO - 1) {
        ic_error("session overflow\n");
        return;
    }

    info_num++;
    pi = &info[info_num];

    pi->lines = 1;
    pi->unget_token = false;

    strncpyz(pi->parse_file, name, sizeof(pi->parse_file));
}

IC_PUBLIC
void ps_end_session(void)
{
    if (info_num == 0) {
        ic_error("session underflow\n");
        return;
    }

    info_num--;
    pi = &info[info_num];
}

IC_PUBLIC
void ps_reset(void)
{
    pi = &info[0];
    info_num = 0;
}
