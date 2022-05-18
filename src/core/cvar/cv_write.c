#include <stdio.h>

#include "iocod.h"

#define fs_printf(f, s, ...) ic_printf(s, __VA_ARGS__)

extern struct cvar *cvars;

IC_PUBLIC
bool cv_write_defaults(filehandle f)
{
    UNUSED_PARAM(f);

    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        /* don't write cd key */
        if (strcasecmp(v->name, "cl_cdkey") == 0)
            continue;

        /* don't write out cvars with these flags */
        #define CV_MASK (CV_ROM | CV_USER_CREATED | CV_CHEAT | CV_4096)
        if ((v->flags & CV_MASK) == 0) {
            char buf[1024];
            snprintfz(buf, sizeof(buf), "set %s \"%s\"", v->name, 
                      v->reset_string);

            fs_printf(f, "%s\n", buf);
        }
    }

    return true;
}

IC_PUBLIC
bool cv_write_variables(filehandle f)
{
    UNUSED_PARAM(f);

    for (struct cvar *v = cvars; v != NULL; v = v->next) {
        /* don't write cd key */
        if (strcasecmp(v->name, "cl_cdkey") == 0)
            continue;

        /* only write archived cvars */
        if (v->flags & CV_ARCHIVE) {
            char buf[1024];

            if (v->latched_string != NULL) {
                snprintf(buf, sizeof(buf), "seta %s \"%s\"", v->name,
                         v->latched_string);
            } else {
                snprintfz(buf, sizeof(buf), "seta %s \"%s\"", v->name,
                          v->string);
            }

            fs_printf(f, "%s\n", buf);
        }
    }

    return true;
}
