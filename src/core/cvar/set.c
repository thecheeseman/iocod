/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

#include <stdlib.h>

#include "iocod.h"

extern int modified_flags;
extern struct cvar *sv_console_lockout;
extern struct cvar *com_sv_running;

static struct cvar *update_cvar(struct cvar *v, const char *name, 
                                const char *value, bool force)
{
    modified_flags |= v->flags;

    if (!force) {
        if (v->flags & CV_ROM) {
            log_print(_("Cvar '%s' is read only\n"), name);
            goto out;
        }

        if (v->flags & CV_INIT) {
            log_print(_("Cvar '%s' is write protected\n"), name);
            goto out;
        }

        if (v->flags & CV_CHEAT) {
            log_print(_("Cvar '%s' is cheated protected\n"), name);
            goto out;
        }
        #if 0
        /* added in 1.5 */
        if (sv_console_lockout->integer != 0 && com_sv_running->integer != 0) {
            log_print(_("Console is locked out, can't change cvars\n"));
            goto out;
        }
        #endif
        if (v->flags & CV_LATCH) {
            if (v->latched_string != NULL) {
                if (strcmp(value, v->latched_string) == 0)
                    goto out;
                
                ic_free(v->latched_string);
            } else {
                if (strcmp(value, v->string) == 0)
                    goto out;
            }

            log_print(_("Cvar '%s' will be changed on restart\n"), name);

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
    log_debug(_("Cvar set %s '%s'"), name, value);

    if (!cv_validate_string(name)) {
        log_error(_("Invalid cvar name '%s'"), name);
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

IC_PUBLIC
struct cvar *cv_set_string(const char *name, const char *value)
{
    return cv_set2(name, value, true);
}

IC_PUBLIC
struct cvar *cv_set_string_latched(const char *name, const char *value)
{
    return cv_set2(name, value, false);
}

IC_PUBLIC
struct cvar *cv_set_value(const char *name, cv_float value)
{
    char v[64];

    snprintfz(v, sizeof(v), "%lf", value);
    return cv_set_string(name, v);
}

IC_PUBLIC
struct cvar *cv_set_integer(const char *name, cv_int value)
{
    char v[64];

    snprintf(v, sizeof(v), "%d", value);
    return cv_set_string(name, v);
}

IC_PUBLIC
struct cvar *cv_reset(const char *name)
{
    return cv_set2(name, NULL, false);
}
