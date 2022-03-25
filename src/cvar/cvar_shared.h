#ifndef CVAR_CVAR_SHARED_H
#define CVAR_CVAR_SHARED_H

#include "cvar/cvar.h"

#if defined(HAVE_CONFIG_H) && defined(HAVE_STDINT_H)
#include <stdint.h>
#else
typedef int int32_t;
#endif

extern struct cvar *cvar_vars;
extern struct cvar *cvar_cheats;

extern struct cvar cvar_indexes[MAX_CVARS];
extern int cvar_num_indexes;
extern struct cvar *hashtable[FILE_HASH_SIZE];

void cvar_add_commands(void);

int32_t generate_hash_value(const char *fname);

/**
 * @brief Internal hash table lookup
*/
struct cvar *cvar_find_var(const char *var_name);

#endif /* CVAR_CVAR_SHARED_H */
