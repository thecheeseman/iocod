#include <sys/stat.h>
#include "conf_local.h"

bool conf_write_defaults(struct config *cfg)
{
    struct stat st;
    if (stat(cfg->filename, &st) == 0)
        return true; // file exists, so skip writing defaults

    FILE *fp = fopen(cfg->filename, "wb");
    if (fp == NULL) {
        conf_set_error(CFG_ERR_FOPEN, strerror(errno));
        return false;
    }

    for (struct configopt *opt = cfg->options; opt->type != CFG_END; opt++) {
        switch (opt->type) {
        case CFG_BLANK:
            break;
        case CFG_HEADER:
            fprintf(fp, "////////////////////////////////////////\n");
            fprintf(fp, "// %s\n", opt->value.s);
            fprintf(fp, "////////////////////////////////////////\n");
            break;
        case CFG_COMMENT:
            fprintf(fp, "// %s", opt->value.s);
            break;
        case CFG_BOOL:
        case CFG_STRING:
            fprintf(fp, "%-32s \"%s\"", opt->name, opt->default_str);
            break;
        case CFG_INT:
            fprintf(fp, "%-32s %ld", opt->name, opt->value.i);
            break;
        case CFG_FLOAT:
            fprintf(fp, "%-32s %f", opt->name, opt->value.f);
            break;
        default:
            conf_set_error(CFG_ERR_UNKNOWN_TYPE, opt->type);
            return false;
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
    return true;
}
