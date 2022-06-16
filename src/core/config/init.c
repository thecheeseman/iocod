#include "conf_local.h"

IC_PUBLIC
struct config *config_init(const char *filename, struct configopt *options)
{
    if (filename == NULL) {
        conf_set_error(CFG_ERR_NULL_FILENAME);
        ic_error("%s", conf_error_string());
        return NULL;
    }

    if (options == NULL) {
        conf_set_error(CFG_ERR_NULL_OPTIONS);
        ic_error("%s", conf_error_string());
        return NULL;
    }

    struct config *cfg = ic_malloc(sizeof(struct config));
    cfg->filename = strdup(filename);
    cfg->options = options;

    if (!conf_set_defaults(cfg) || !conf_write_defaults(cfg) ||
        !conf_load(cfg) || !conf_parse(cfg)) {
        ic_free(cfg->filename);
        ic_free(cfg);
        ic_error("%s", conf_error_string());
        return NULL;
    }

    conf_dump_options(cfg);

    return cfg;
}

IC_PUBLIC
void config_shutdown(void)
{

}
