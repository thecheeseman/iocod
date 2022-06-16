#include <errno.h>
#include "conf_local.h"

static int conferrno = CFG_ERR_OK;

static char *errstrs[] = {
    "OK",
    "NULL conf pointer",
    "NULL filename",
    "NULL options pointer",
    "Error while stat(): %s",
    "Error while fopen(): %s",
    "Error while fread(): %s",
    "Line %d is incomplete",
    "Token too large on line %d",
    "Expected value for token '%s'",
    "Unknown value for boolean: %s",
    "Unknown config variable type %d"
};

static char conferrmsg[1024];
void conf_set_error(int err, ...)
{
    conferrno = err;

    va_list argptr;
    va_start(argptr, err);
    vsnprintf(conferrmsg, sizeof(conferrmsg), errstrs[conferrno], argptr);
    va_end(argptr);
}

char *conf_error_string(void)
{
    return conferrmsg;
}
