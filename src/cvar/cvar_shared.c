#include <ctype.h>

#include "shared.h"

#include "common/error.h"
#include "cvar/cvar_shared.h"
#include "strings/stringlib.h"
#include "types/null.h"

struct cvar *hashtable[FILE_HASH_SIZE];

int32_t generate_hash_value(const char *fname)
{
    int i;
    int32_t hash;
    char letter;

    if (fname == NULL)
        com_error(ERR_DROP, "null name");

    hash = 0, i = 0;
    while (fname[i] != '\0') {
        letter = tolower(fname[i]);
        hash += (int32_t) (letter) * (i + 119);
        i++;
    }

    hash &= (FILE_HASH_SIZE - 1);
    return hash;
}

struct cvar *cvar_find_var(const char *var_name)
{
    struct cvar *var;
    int32_t hash;

    hash = generate_hash_value(var_name);

    for (var = hashtable[hash]; var != NULL; var = var->hash_next) {
        if (var->name == NULL)
            continue; // edge case during cvar_restart

        if (strcasecmp(var_name, var->name) == 0)
            return var;
    }

    return NULL;
}
