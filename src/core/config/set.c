#include "conf_local.h"

IC_PUBLIC
bool conf_set_bool(struct conf *cfg, const char *name, bool value)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_BOOL)
        return false;

    opt->value.i = value;
    return true;
}

IC_PUBLIC
bool conf_set_int(struct conf *cfg, const char *name, conf_int value)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_INT)
        return false;

    opt->value.i = value;
    return true;
}

IC_PUBLIC
bool conf_set_float(struct conf *cfg, const char *name, conf_float value)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_FLOAT)
        return false;

    opt->value.f = value;
    return true;
}

IC_PUBLIC
bool conf_set_string(struct conf *cfg, const char *name, char *value)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_STRING)
        return false;

    if (opt->value.s != NULL)
        ic_free(opt->value.s);

    opt->value.s = strdup(value);
    return true;
}
