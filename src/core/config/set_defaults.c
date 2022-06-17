#include "conf_local.h"

bool conf_set_defaults(struct conf *cfg)
{
    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        switch (opt->type) {
        case CONF_BOOL:
            if (strcasecmp(opt->default_str, "yes") == 0 ||
                strcasecmp(opt->default_str, "true") == 0 ||
                strcasecmp(opt->default_str, "on") == 0) {
                opt->value.i = 1;
            } else if (strcasecmp(opt->default_str, "no") == 0 ||
                       strcasecmp(opt->default_str, "false") == 0 ||
                       strcasecmp(opt->default_str, "off") == 0) {
                opt->value.i = 0;
            } else {
                conf_set_error(CONF_ERR_UNKNOWN_BOOLEAN, opt->default_str);
                return false;
            }
            break;
        case CONF_INT:
            opt->value.i = (conf_int) strtol(opt->default_str, NULL, 10);
            break;
        case CONF_FLOAT:
            opt->value.f = (conf_float) strtod(opt->default_str, NULL);
            break;
        case CONF_STRING:
        case CONF_HEADER:
        case CONF_COMMENT:
            opt->value.s = strdup(opt->default_str);
            break;
        case CONF_BLANK:
        case CONF_END:
            break;
        default:
            conf_set_error(CONF_ERR_UNKNOWN_TYPE, opt->type);
            return false;
        }
    }

    return true;
}
