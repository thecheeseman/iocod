#include "conf_local.h"

static bool end_of_script(struct config *cfg, bool crossline)
{
    if (!crossline) {
        conf_set_error(CFG_ERR_INCOMPLETE, cfg->script_line);
        return false;
    }

    cfg->end = true;
    ic_free(cfg->buffer);
    return false;
}

static bool get_token(struct config *cfg, bool crossline)
{
    if (cfg->script_p >= cfg->end_p)
        return end_of_script(cfg, crossline);

skipspace:
    while (*cfg->script_p <= ' ') {
        if (cfg->script_p >= cfg->end_p)
            return end_of_script(cfg, crossline);

        if (*cfg->script_p++ == '\n') {
            if (!crossline) {
                conf_set_error(CFG_ERR_INCOMPLETE, cfg->script_line);
                return false;
            }

            cfg->script_line = cfg->line++;
        }
    }

    if (cfg->script_p >= cfg->end_p)
        return end_of_script(cfg, crossline);

    // handle ; # // comments
    if (*cfg->script_p == ';' ||
        *cfg->script_p == '#' ||
        (cfg->script_p[0] == '/' && cfg->script_p[1] == '/')) {
        if (!crossline) {
            conf_set_error(CFG_ERR_INCOMPLETE, cfg->script_line);
            return false;
        }

        while (*cfg->script_p++ != '\n')
            if (cfg->script_p >= cfg->end_p)
                return end_of_script(cfg, crossline);

        cfg->script_line = cfg->line++;
        goto skipspace;
    }

    char *token_p = cfg->token;
    // handle quotes
    if (*cfg->script_p == '"') {
        cfg->script_p++;

        while (*cfg->script_p != '"') {
            *token_p++ = *cfg->script_p++;

            if (cfg->script_p >= cfg->end_p)
                break;

            if (token_p == &cfg->token[MAX_CONFIG_TOKEN]) {
                conf_set_error(CFG_ERR_TOKEN_TOO_LARGE, cfg->script_line);
                return false;
            }
        }

        cfg->script_p++;
    } else {
        while (*cfg->script_p > 32) {
            *token_p++ = *cfg->script_p++;

            if (cfg->script_p >= cfg->end_p)
                break;

            if (token_p == &cfg->token[MAX_CONFIG_TOKEN]) {
                conf_set_error(CFG_ERR_TOKEN_TOO_LARGE, cfg->script_line);
                return false;
            }
        }
    }

    *token_p = 0;

    return true;
}

bool conf_parse(struct config *cfg)
{
    while (!cfg->end) {
        // out of tokens
        if (!get_token(cfg, true))
            break;

        char optname[MAX_TOKEN_CHARS];
        strncpyz(optname, cfg->token, sizeof(optname));

        bool valid = false;
        struct configopt *opt;
        for (opt = cfg->options; opt->type != CFG_END; opt++) {
            // skip blanks, comments, headers
            if (opt->type == CFG_BLANK || opt->type == CFG_HEADER ||
                opt->type == CFG_COMMENT) {
                continue;
            }

            // got it
            if (strcasecmp(opt->name, optname) == 0) {
                valid = true;
                break;
            }
        }

        if (!get_token(cfg, false)) {
            conf_set_error(CFG_ERR_EXPECTED_TOKEN, optname);
            return false; // stop parsing
        }

        // copy data
        char *str;
        switch (opt->type) {
        case CFG_BOOL:
            if (strcasecmp(cfg->token, "yes") == 0 ||
                strcasecmp(cfg->token, "true") == 0) {
                opt->value.i = 1;
            } else if (strcasecmp(cfg->token, "no") == 0 ||
                       strcasecmp(cfg->token, "false") == 0) {
                opt->value.i = 0;
            } else {
                conf_set_error(CFG_ERR_UNKNOWN_BOOLEAN, cfg->token);
                return false;
            }
            break;
        case CFG_INT:
            opt->value.i = strtol(cfg->token, NULL, 10);
            break;
        case CFG_FLOAT:
            opt->value.f = strtod(cfg->token, NULL);
            break;
        case CFG_STRING:
        case CFG_HEADER:
        case CFG_COMMENT:
            str = strdup(cfg->token);
            opt->value.s = str;
            break;
        }
    }

    return true;
}
