#include "conf_local.h"

IC_PUBLIC
struct confopt *conf_get_opt(struct conf *cfg, const char *name)
{
    if (cfg == NULL || name == NULL)
        return NULL;

    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        if (strcasecmp(opt->name, name) == 0)
            return opt;
    }

    return NULL;
}

IC_PUBLIC
bool conf_get_bool(struct conf *cfg, const char *name)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_BOOL)
        return false;

    return (opt->value.i != 0);
}

IC_PUBLIC
conf_int conf_get_int(struct conf *cfg, const char *name)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_INT)
        return 0;

    return opt->value.i;
}

IC_PUBLIC
conf_float conf_get_float(struct conf *cfg, const char *name)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_FLOAT)
        return 0.0;

    return opt->value.f;
}

IC_PUBLIC
char *conf_get_string(struct conf *cfg, const char *name)
{
    struct confopt *opt = conf_get_opt(cfg, name);
    if (opt == NULL || opt->type != CONF_STRING)
        return NULL;

    return opt->value.s;
}
