#include <ctype.h>
#include <stdlib.h>

#include "iocod.h"

struct cvar *cvars;
extern struct cvar *cv_cheats;

static struct cvar indexes[MAX_CVARS];
static size_t num_indexes = 0;

static struct cvar *hashtable[MAX_CVARS];

int modified_flags = 0;

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
        hash += (long)(c) * (i + 119);
        i++;
    }

    hash &= (MAX_VMCVAR_STRING_LEN - 1);
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
    if (hash >= MAX_CVARS || hash <= 0) {
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
    #define CV_MASK (CV_USER_CREATED | CV_4096)
    if ((v->flags & CV_MASK) != 0 && (flags & CV_MASK) == 0 &&
        ((*value != '\0') || (flags & CV_CHEAT) != 0)) {

        v->flags &= ~CV_MASK;

        ic_free(v->reset_string);
        v->reset_string = strdup(value);

        /* 
            ZOID--needs to be set so that cvars the game sets as
            SERVERINFO get sent to clients 
        */
        modified_flags |= flags;
    }

    v->flags |= flags;

    /* only allow one non-empty reset string without a warning */
    if (*v->reset_string != '\0') {
        /* none yet */
        ic_free(v->reset_string);

        v->reset_string = strdup(value);
    } else if (*value != '\0' && strcmp(v->reset_string, value) != 0) {
        log_warn(_("Cvar '%1$s' given initial values: '%2$s' and '%3$s'"),
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
}

/*
 * create a cvar.
 */
static struct cvar *create_cvar(const char *name, const char *value,
                                enum cv_flags flags)
{
    struct cvar *v = &indexes[num_indexes];
    num_indexes++;

    v->name = strdup(name);
    v->flags = flags;
    
    v->string = strdup(value);
    v->reset_string = strdup(value);
    v->value = (cv_float) strtod(value, NULL);
    v->integer = (cv_int) strtol(value, NULL, 10);

    v->modified = true;
    v->modification_count = 1;

    /* link the next var */
    v->next = cvars;
    cvars = v;

    /* hashtable */
    long hash = cv_hash(name);
    v->hash_next = hashtable[hash];
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

    if (num_indexes >= MAX_CVARS) {
        log_error(_("Reached maximum number of cvars"));
        return NULL;
    }

    return create_cvar(name, value, flags);
}

