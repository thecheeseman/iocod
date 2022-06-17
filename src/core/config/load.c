#include <sys/stat.h>
#include "conf_local.h"

bool conf_load(struct conf *cfg)
{
    if (cfg == NULL) {
        conf_set_error(CONF_ERR_NULL_CFG);
        return false;
    }

    if (cfg->filename == NULL) {
        conf_set_error(CONF_ERR_NULL_FILENAME);
        return false;
    }

    struct stat st;
    if (stat(cfg->filename, &st) != 0) {
        conf_set_error(CONF_ERR_STAT, strerror(errno));
        return false;
    }

    FILE *fp = fopen(cfg->filename, "rb");
    if (fp == NULL) {
        conf_set_error(CONF_ERR_FOPEN, strerror(errno));
        return false;
    }

    fseek(fp, 0, SEEK_END);
    cfg->size = (size_t) ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buf = ic_malloc(cfg->size + 1);
    memset(buf, 0, cfg->size + 1);

    if (fread(buf, 1, cfg->size, fp) != cfg->size) {
        ic_free(buf);
        fclose(fp);

        conf_set_error(CONF_ERR_FREAD, strerror(errno));
        return false;
    }
    fclose(fp);

    cfg->line = 1;
    cfg->buffer = buf;
    cfg->script_p = cfg->buffer;
    cfg->end_p = cfg->buffer + cfg->size;

    return true;
}
