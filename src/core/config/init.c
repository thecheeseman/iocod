#include "conf_local.h"

IC_PUBLIC
struct conf *conf_init(const char *filename, struct confopt *kv, int options)
{
    UNUSED_PARAM(options);

    if (filename == NULL) {
        conf_set_error(CONF_ERR_NULL_FILENAME);
        ic_error("%s", conf_error_string());
        return NULL;
    }

    if (kv == NULL) {
        conf_set_error(CONF_ERR_NULL_OPTIONS);
        ic_error("%s", conf_error_string());
        return NULL;
    }

    struct conf *cfg = ic_calloc(1, sizeof(struct conf));
    cfg->filename = strdup(filename);
    cfg->options = kv;

    if (!conf_set_defaults(cfg) || 
        !conf_write_defaults(cfg) ||
        !conf_load(cfg) || 
        !conf_parse(cfg)) {
        conf_shutdown(cfg);
        ic_error("%s", conf_error_string());
        return NULL;
    }

    return cfg;
}

IC_PUBLIC
bool conf_shutdown(struct conf *cfg)
{
    if (cfg == NULL)
        return true; // already closed

    ic_free(cfg->filename);

    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        if (opt->type == CONF_STRING || opt->type == CONF_HEADER ||
            opt->type == CONF_COMMENT) {
            ic_free(opt->value.s);
        }
    }

    ic_free(cfg);

    return true;
}
