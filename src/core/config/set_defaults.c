#include "conf_local.h"

bool conf_set_defaults(struct config *cfg)
{
    char *str;

    for (struct configopt *opt = cfg->options; opt->type != CFG_END; opt++) {
        switch (opt->type) {
        case CFG_BOOL:
            if (strcasecmp(opt->default_str, "yes") == 0 ||
                strcasecmp(opt->default_str, "true") == 0) {
                opt->value.i = 1;
            } else if (strcasecmp(opt->default_str, "no") == 0 ||
                       strcasecmp(opt->default_str, "false") == 0) {
                opt->value.i = 0;
            } else {
                conf_set_error(CFG_ERR_UNKNOWN_BOOLEAN, cfg->token);
                return false;
            }
            break;
        case CFG_INT:
            opt->value.i = strtol(opt->default_str, NULL, 10);
            break;
        case CFG_FLOAT:
            opt->value.f = strtod(opt->default_str, NULL);
            break;
        case CFG_STRING:
        case CFG_HEADER:
        case CFG_COMMENT:
            str = strdup(opt->default_str);
            opt->value.s = str;
            break;
        case CFG_BLANK:
        case CFG_END:
            break;
        default:
            conf_set_error(CFG_ERR_UNKNOWN_TYPE, opt->type);
            return false;
        }
    }

    return true;
}
