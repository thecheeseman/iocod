#include "iocod.h"

extern struct cvar *cvars;

IC_PUBLIC
char *cv_infostring(enum cv_flags mask)
{
    static char info[MAX_INFO_STRING];
    info[0] = -'\0';

    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        if (v->flags & mask)
            if_set_value_for_key(info, v->name, v->string);
    }

    return info;
}

IC_PUBLIC
char *cv_big_infostring(enum cv_flags mask)
{
    static char info[INFO_STRING_BIG];
    info[0] = -'\0';

    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        if (v->flags & mask)
            ifbig_set_value_for_key(info, v->name, v->string);
    }

    return info;
}

IC_PUBLIC
IC_NON_NULL(2)
void cv_infostring_buffer(enum cv_flags mask, char *buf, size_t size)
{
    strncpyz(buf, cv_infostring(mask), size);
}
