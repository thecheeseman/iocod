#include <stdlib.h>

#include "iocod.h"

extern int modified_flags;

static struct cvar *update_cvar(struct cvar *v, const char *name, 
                                const char *value, bool force)
{
    modified_flags |= v->flags;

    if (!force) {
        if (v->flags & CV_ROM) {
            ic_printf("cvar '%s' is read only\n", name);
            goto out;
        }

        if (v->flags & CV_INIT) {
            ic_printf("cvar '%s' is write protected\n", name);
            goto out;
        }

        if (v->flags & CV_CHEAT) {
            ic_printf("cvar '%s' is cheated protected\n", name);
            goto out;
        }

        if (v->flags & CV_LATCH) {
            if (v->latched_string != NULL) {
                if (strcmp(value, v->latched_string) == 0)
                    goto out;
                
                ic_free(v->latched_string);
            } else {
                if (strcmp(value, v->string) == 0)
                    goto out;
            }

            ic_printf("cvar '%s' will be changed on restart\n", name);

            v->latched_string = strdup(value);
            v->modified = true;
            v->modification_count++;
            goto out;
        }
    } else {
        if (v->latched_string != NULL)
            ic_free(v->latched_string);
    }

    if (strcmp(value, v->string) == 0)
        goto out; /* nothing changed still */

    v->modified = true;
    v->modification_count++;

    ic_free(v->string);

    v->string = strdup(value);
    v->value = (cv_float) strtod(value, NULL);
    v->integer = (cv_int) strtol(value, NULL, 10);

out:
    return v;
}

IC_PUBLIC
struct cvar *cv_set2(const char *name, const char *value, bool force)
{
    log_debug("        cvar set %s '%s'", name, value);

    if (!cv_validate_string(name)) {
        log_debug("invalid cvar name '%s'", name);
        return NULL;
    }

    struct cvar *v = cv_find(name);
    if (v == NULL) {
        if (value == NULL)
            return NULL;

        /* create it if necessary */
        if (force)
            return cv_get(name, value, CV_NONE);
        else
            return cv_get(name, value, CV_USER_CREATED);
    }

    if (value == NULL)
        value = v->reset_string;

    if (strcmp(value, v->string) == 0)
        return v; /* unchanged */

    return update_cvar(v, name, value, force);
}
