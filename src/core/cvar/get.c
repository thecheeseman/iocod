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

#include <ctype.h>
#include <stdlib.h>

#include "cvar_local.h"

struct cvar *cvars;

struct cvar indexes[MAX_CVARS];
size_t num_indexes = 0;
struct cvar *hashtable[FILE_HASH_SIZE];

int cv_modified_flags = 0;

/*
 * Return a hash for a given cvar name.
 */
static long cv_hash(const char *name)
{
    if (name == NULL || *name == '\0') {
        log_warn(_("Got NULL value for parameter 'name'\n"));
        return -1;
    }

    long hash = 0;
    long i = 0;
    while (name[i] != '\0') {
        char c = tolower(name[i]);
        hash += (long) (c) * (i + 119);
        i++;
    }

    hash &= (FILE_HASH_SIZE - 1);
    return hash;
}

IC_PUBLIC
struct cvar *cv_find(const char *name)
{
    if (name == NULL || *name == '\0') {
        log_trace(_("Got NULL value for parameter 'name'\n"));
        return NULL;
    }

    long hash = cv_hash(name);
    if (hash >= FILE_HASH_SIZE || hash <= 0) {
        log_trace(_("Bad hash %ld for name '%s'\n"), hash, name);
        return NULL;
    }

    for (struct cvar *v = hashtable[hash]; v != NULL; v = v->hash_next) {
        if (v->name == NULL)
            continue;

        if (strcasecmp(name, v->name) == 0)
            return v;
    }

    return NULL;
}

/*
 * update cvar flags / reset strings etc.
 */
static void update_cvar(struct cvar *v, const char *name, const char *value,
                        enum cv_flags flags)
{
    /*
        if the C code is now specifying a variable that the user already
        set a value for, take the new value as the reset value
    */
    if (v->flags & CV_USER_CREATED) {
        v->flags &= ~CV_USER_CREATED;

        ic_free(v->reset_string);
        v->reset_string = copy_string(value);

        if (flags & CV_ROM) {
            ic_free(v->latched_string);
            v->latched_string = copy_string(value);
        }
    }

    v->flags |= flags;

    /* only allow one non-empty reset string without a warning */
    if (v->reset_string[0] == '\0') {
        /* none yet */
        ic_free(v->reset_string);

        v->reset_string = copy_string(value);
    } else if (value[0] != '\0' && strcmp(v->reset_string, value) != 0) {
        log_warn(_("Cvar '%s' given initial values: '%s' and '%s'"),
                 name, v->reset_string, value);
    }

    /* if we have a latched string, take that value now */
    if (v->latched_string != NULL) {
        char *s = v->latched_string;
        v->latched_string = NULL;

        cv_set2(name, s, true);
        ic_free(s);
    }

    if ((v->flags & CV_CHEAT) != 0 && cv_cheats != NULL &&
        cv_cheats->integer == 0) {
        cv_set2(name, value, true);
    }

    /*
        ZOID--needs to be set so that cvars the game sets as
        SERVERINFO get sent to clients
    */
    cv_modified_flags |= flags;
}

/*
 * create a cvar.
 */
static struct cvar *create_cvar(size_t index, const char *name, 
                                const char *value, enum cv_flags flags)
{
    struct cvar *v = &indexes[index];

    if (index >= num_indexes)
        num_indexes = index + 1;

    v->name = copy_string(name);
    v->flags = flags;
    cv_modified_flags |= v->flags;

    v->string = copy_string(value);
    v->reset_string = copy_string(value);
    v->value = (cv_float) strtod(value, NULL);
    v->integer = (cv_int) strtol(value, NULL, 10);

    v->modified = true;
    v->modification_count = 1;

    v->description = NULL;

    /* link the next var */
    v->next = cvars;
    if (cvars != NULL)
        cvars->prev = v;

    v->prev = NULL;
    cvars = v;

    /* hashtable */
    long hash = cv_hash(name);
    v->hash_index = hash;
    v->hash_next = hashtable[hash];
    if (hashtable[hash] != NULL)
        hashtable[hash]->hash_prev = v;

    v->hash_prev = NULL;
    hashtable[hash] = v;

    return v;
}

IC_PUBLIC
struct cvar *cv_get(const char *name, const char *value, enum cv_flags flags)
{
    if (name == NULL || value == NULL) {
        log_error(_("Got NULL value for parameter 'value'"));
        return NULL;
    }

    if (!cv_validate_string(name)) {
        log_error(_("Invalid cvar name '%s'"), name);
        return NULL;
    }

    struct cvar *v = cv_find(name);
    if (v != NULL) {
        update_cvar(v, name, value, flags);
        return v;
    }

    // ioq3 find the next open cvar
    size_t index;
    for (index = 0; index < MAX_CVARS; index++) {
        if (indexes[index].name == NULL)
            break;
    }

    if (index >= MAX_CVARS) {
        log_error(_("Reached maximum number of cvars"));
        return NULL;
    }

    return create_cvar(index, name, value, flags);
}

