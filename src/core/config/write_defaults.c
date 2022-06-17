#include <sys/stat.h>
#include "conf_local.h"

bool conf_write_defaults(struct conf *cfg)
{
    struct stat st;
    if (stat(cfg->filename, &st) == 0)
        return true; // file exists, so skip writing defaults

    FILE *fp = fopen(cfg->filename, "wb");
    if (fp == NULL) {
        conf_set_error(CONF_ERR_FOPEN, strerror(errno));
        return false;
    }

    char *comment = "#";
    if (cfg->comment_style == CONF_COMMENTSTYLE_CXX)
        comment = "//";
    else if (cfg->comment_style == CONF_COMMENTSTYLE_INF)
        comment = ";";

    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        switch (opt->type) {
        case CONF_BLANK:
            break;
        case CONF_HEADER:
            fprintf(fp, "%s\n", comment);
            fprintf(fp, "%s %s\n", comment, opt->value.s);
            fprintf(fp, "%s\n", comment);
            break;
        case CONF_COMMENT:
            fprintf(fp, "%s %s", comment, opt->value.s);
            break;
        case CONF_BOOL:
            fprintf(fp, "%s %s", opt->name, opt->default_str);
            break;
        case CONF_STRING:
            fprintf(fp, "%s \"%s\"", opt->name, opt->default_str);
            break;
        case CONF_INT:
            fprintf(fp, "%s %ld", opt->name, opt->value.i);
            break;
        case CONF_FLOAT:
            fprintf(fp, "%s %g", opt->name, opt->value.f);
            break;
        default:
            conf_set_error(CONF_ERR_UNKNOWN_TYPE, opt->type);
            return false;
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
    return true;
}
