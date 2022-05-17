#include "iocod.h"

IC_PUBLIC
char *cv_clear_foreign_chars(const char *value)
{
    if (value == NULL)
        return "";

    static char clean[MAX_VMCVAR_STRING_LEN]; /* should this be larger? */

    int j = 0;
    for (int i = 0; value[i] != '\0'; i++) {
        if ((value[i] & 128) == 0) {
            clean[j] = value[i];
            j++;
        }
    }
    clean[j] = '\0';

    return clean;
}

static char *get_string(const char *name)
{
    if (name == NULL || *name == '\0') {
        log_trace("got NULL name");
        return NULL;
    }

    struct cvar *v = cv_find(name);
    if (v == NULL) {
        log_trace("unable to find cvar '%s'", name);
        return NULL;
    }

    return v->string;
}

IC_PUBLIC
IC_NON_NULL(1)
char *cv_string(const char *name)
{
    char *str = get_string(name);

    if (str == NULL)
        return "";
    else
        return str;
}

IC_PUBLIC
IC_NON_NULL(1, 2)
void cv_string_buffer(const char *name, char *buf, size_t size)
{
    char *str = get_string(name);

    if (str == NULL)
        *buf = '\0';
    else
        strncpyz(buf, str, size);
}

IC_PUBLIC
bool cv_validate_string(const char *s)
{
    if (s == NULL || strchr(s, '\\') || strchr(s, '\"') || strchr(s, ';'))
        return false;

    return true;
}
