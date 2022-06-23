#include "iocod.h"

union cv_value {
    cv_float f;
    cv_int i;
};

static union cv_value get_value(const char *name, bool f)
{
    union cv_value u = { 0 };

    if (name == NULL || *name == '\0') {
        log_trace(_("Got NULL value for parameter 'name'\n"));
        return u;
    }

    struct cvar *v = cv_find(name);
    if (v == NULL) {
        log_trace(_("Unable to find cvar '%s'\n"), name);
        return u;
    }

    if (f)
        u.f = v->value;
    else
        u.i = v->integer;

    return u;
}

IC_PUBLIC
IC_NON_NULL(1)
cv_float cv_get_value(const char *name)
{
    union cv_value u = get_value(name, true);
    return u.f;
}

IC_PUBLIC
IC_NON_NULL(1)
cv_int cv_get_integer(const char *name)
{
    union cv_value u = get_value(name, false);
    return u.i;
}
