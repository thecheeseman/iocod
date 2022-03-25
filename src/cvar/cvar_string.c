#if defined(HAVE_CONFIG_H) && defined(HAVE_STRING_H)
#include <string.h>
#endif

#include "shared.h"
#include "cvar/cvar_shared.h"

/**
 * @brief Validate a cvar string
 * @param s String to validate
 * @return False if string contains '\' or '"' or ';' chars, true otherwise
*/
bool cvar_validate_string(const char *s)
{
    if (s == NULL)
        return false;

    if (strchr(s, '\\'))
        return false;

    if (strchr(s, '\"'))
        return false;

    if (strchr(s, ';'))
        return false;

    return true;
}

char *cvar_variable_string(const char *var_name)
{
    struct cvar *var;

    var = cvar_find_var(var_name);
    if (var == NULL)
        return "";

    return var->string;
}

void cvar_variable_string_buffer(const char *var_name, char *buffer, int bufsize)
{
    struct cvar *var;

    var = cvar_find_var(var_name);
    if (var == NULL)
        *buffer = 0;
    else
        q_strncpyz(buffer, var->string, bufsize);
}

char *cvar_clean_foreign_characters(const char *value)
{
    static char clean[MAX_CVAR_VALUE_STRING];
    int i, j;

    j = 0;
    for (i = 0; value[i] != '\0'; i++) {
        if (!(value[i] & 128)) {
            clean[j] = value[i];
            j++;
        }
    }
    clean[j] = '\0';

    return clean;
}

char *cvar_info_string(int bit)
{
    static char info[MAX_INFO_STRING];
    struct cvar *var;

    info[0] = '\0';

    for (var = cvar_vars; var != NULL; var = var->next) {
        if (var->flags & bit)
            info_set_value_for_key(info, var->name, var->string);
    }

    return info;
}

char *cvar_info_string_big(int bit)
{
    static char info[BIG_INFO_STRING];
    struct cvar *var;

    info[0] = '\0';

    for (var = cvar_vars; var != NULL; var = var->next) {
        if (var->flags & bit)
            info_set_value_for_key_big(info, var->name, var->string);
    }

    return info;
}
