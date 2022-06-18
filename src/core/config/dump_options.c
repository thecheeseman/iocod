#include "conf_local.h"

static const char *opttypestr[] = {
    "",
    "comment",
    "header",
    "section",
    "boolean",
    "integer",
    "float",
    "string",
    ""
};

IC_PUBLIC
void conf_dump_options(struct conf *cfg)
{
    printf("%-16s %-32s %s\n", "type", "name", "value");

    for (struct confopt *opt = cfg->options; opt->type != CONF_END; opt++) {
        char value[256];
        bool print_name = true;

        switch (opt->type) {
        case CONF_HEADER:
        case CONF_SECTION:
        case CONF_COMMENT:
            print_name = false;

            if (opt->type == CONF_COMMENT)
                snprintf(value, sizeof(value), "# %s", opt->value.s);
            else
                snprintf(value, sizeof(value), "%s", opt->value.s);
            break;
        case CONF_BOOL:
            if (opt->value.i)
                snprintf(value, sizeof(value), "true");
            else
                snprintf(value, sizeof(value), "false");
            break;
        case CONF_INT:
            snprintf(value, sizeof(value), "%d", opt->value.i);
            break;
        case CONF_FLOAT:
            snprintf(value, sizeof(value), "%g", opt->value.f);
            break;
        case CONF_STRING:
            snprintf(value, sizeof(value), "\"%s\"", opt->value.s);
            break;
        }

        if (opt->type == CONF_BLANK) {
            printf("\n");
        } else {
            printf("%-16s %-32s %s\n",
                   opt->type <= CONF_END ? opttypestr[opt->type] : "bad type",
                   print_name ? opt->name : "",
                   value);
        }
    }
}
