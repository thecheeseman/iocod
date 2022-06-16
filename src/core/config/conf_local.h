#ifndef CONFIG_LOCAL_H
#define CONFIG_LOCAL_H

#include "iocod.h"
#include <ctype.h>
#include <stdlib.h>

enum cfgerror_t {
    CFG_ERR_OK,
    CFG_ERR_NULL_CFG,
    CFG_ERR_NULL_FILENAME,
    CFG_ERR_NULL_OPTIONS,
    CFG_ERR_STAT,
    CFG_ERR_FOPEN,
    CFG_ERR_FREAD,
    CFG_ERR_INCOMPLETE,
    CFG_ERR_TOKEN_TOO_LARGE,
    CFG_ERR_EXPECTED_TOKEN,
    CFG_ERR_UNKNOWN_BOOLEAN,
    CFG_ERR_UNKNOWN_TYPE,

    CFG_ERR_UNKNOWN
};

void conf_set_error(int err, ...);
char *conf_error_string(void);

bool conf_load(struct config *cfg);
bool conf_set_defaults(struct config *cfg);
bool conf_write_defaults(struct config *cfg);
bool conf_parse(struct config *cfg);
void conf_dump_options(struct config *cfg);

#endif /* CONFIG_LOCAL_H */
