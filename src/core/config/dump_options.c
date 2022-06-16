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

void conf_dump_options(struct config *cfg)
{
    for (struct configopt *opt = cfg->options; opt->type != CFG_END; opt++) {
        printf("%-32s %-16s ", opt->name,
               opt->type <= CFG_END ? opttypestr[opt->type] : "BAD TYPE");

        switch (opt->type) {
        case CFG_BLANK:
            break;
        case CFG_BOOL:
            if (opt->value.i)
                printf("true");
            else
                printf("false");
            break;
        case CFG_INT:
            printf("%d", opt->value.i);
            break;
        case CFG_FLOAT:
            printf("%f", opt->value.f);
            break;
        case CFG_STRING:
            printf("\"%s\"", opt->value.s);
            break;
        case CFG_COMMENT:
            printf("// %s", opt->value.s);
            break;
        }

        printf("\n");
    }
}
