#include "conf_local.h"

static const char *opttypestr[] = {
    "CFG_NONE",
    "CFG_BLANK",
    "CFG_HEADER",
    "CFG_COMMENT",
    "CFG_BOOL",
    "CFG_INT",
    "CFG_FLOAT",
    "CFG_STRING",
    "CFG_END"
};

IC_PUBLIC
void conf_dump_options(struct conf *cfg)
{
    char *comment = "#";
    if (cfg->comment_style == CONF_COMMENTSTYLE_CXX)
        comment = "//";
    else if (cfg->comment_style == CONF_COMMENTSTYLE_INF)
        comment = ";";

    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        printf("%-32s %-16s ", opt->name,
               opt->type <= CONF_END ? opttypestr[opt->type] : "BAD TYPE");

        switch (opt->type) {
        case CONF_BLANK:
            break;
        case CONF_BOOL:
            if (opt->value.i)
                printf("true");
            else
                printf("false");
            break;
        case CONF_INT:
            printf("%d", opt->value.i);
            break;
        case CONF_FLOAT:
            printf("%f", opt->value.f);
            break;
        case CONF_STRING:
            printf("\"%s\"", opt->value.s);
            break;
        case CONF_COMMENT:
            printf("%s %s", comment, opt->value.s);
            break;
        }

        printf("\n");
    }
}
