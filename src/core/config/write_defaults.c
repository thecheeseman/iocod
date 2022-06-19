#include <sys/stat.h>
#include "conf_local.h"

#ifdef IC_PLATFORM_WINDOWS
#define NL "\r\n"
#else
#define NL "\n"
#endif

static void print_header(FILE *fp, char *header)
{
    size_t len = strnlen(header, 76) + 4;

    for (int i = 0; i < len; i++)
        fwrite("#", 1, 1, fp);

    fprintf(fp, "%s# %s #%s", NL, header, NL);

    for (int i = 0; i < len; i++)
        fwrite("#", 1, 1, fp);

    fprintf(fp, "%s", NL);
}

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

    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        switch (opt->type) {
        case CONF_BLANK:
            break;
        case CONF_HEADER:
            print_header(fp, opt->value.s);
            break;
        case CONF_SECTION:
            fprintf(fp, "#%s# %s%s#", NL, opt->value.s, NL);
            break;
        case CONF_COMMENT:
            fprintf(fp, "# %s", opt->value.s);
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

        fprintf(fp, "%s", NL);
    }

    fclose(fp);
    return true;
}
