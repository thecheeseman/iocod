#include "cvar/cvar_shared.h"
#include "types/null.h"

float cvar_variable_value(const char *var_name)
{
    struct cvar *var;

    var = cvar_find_var(var_name);
    if (var == NULL)
        return 0.0;

    return var->value;
}

int cvar_variable_integer_value(const char *var_name)
{
    struct cvar *var;

    var = cvar_find_var(var_name);
    if (var == NULL)
        return 0;

    return var->integer;
}
