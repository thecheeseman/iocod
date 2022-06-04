#include <sys/stat.h>

#include "iocod.h"
#include "confuse.h"

static cfg_t *cfg;

/**
 * @brief Module opts
*/
static cfg_opt_t module_opts[] = {
    CFG_STR_LIST("enabled", "{ }", CFGF_NONE),
    CFG_END()
};

/**
 * @brief Main config contents
*/
static cfg_opt_t opts[] = {
    CFG_STR("language", "english", CFGF_NONE),

    CFG_BOOL("codextended_compat", cfg_true, CFGF_NONE),
    CFG_INT("cod_version_target", 0, CFGF_NONE),

    CFG_BOOL("autoupdate_enabled", cfg_true, CFGF_NONE),
    CFG_BOOL("autoupdate_on_major_release", cfg_false, CFGF_NONE),
    CFG_BOOL("autoupdate_on_minor_release", cfg_true, CFGF_NONE),
    CFG_BOOL("autoupdate_on_patch_release", cfg_false, CFGF_NONE),
    CFG_BOOL("autoupdate_when_inactive", cfg_false, CFGF_NONE),
    CFG_INT("autoupdate_inactive_delay", 120, CFGF_NONE),

    CFG_SEC("modules", module_opts, CFGF_NONE),
    CFG_END()
};

static void write_default_config(void)
{
    #include "default.inc"

    FILE *fp = fopen("iocod.conf", "wb");
    fwrite(data, sizeof(char), data_len, fp);
    fclose(fp);
}

static void print_values(void)
{
    ic_printf("%-32s %s\n", "config entry", "value");
    ic_printf("----------------------------------------------------------------\n");
    ic_printf("%-32s %s\n", "language", config_get_string("language"));
    ic_printf("%-32s %s\n", "codextended_compat",
              config_get_bool("codextended_compat") ? "true" : "false");
    ic_printf("%-32s %d\n", "cod_version_target", 
              config_get_int("cod_version_target"));
    ic_printf("\n");
    ic_printf("%-32s %s\n", "autoupdate_enabled",
              config_get_bool("autoupdate_enabled") ? "true" : "false");
    ic_printf("%-32s %s\n", "autoupdate_on_major_release",
              config_get_bool("autoupdate_on_major_release") ? "true" : "false");
    ic_printf("%-32s %s\n", "autoupdate_on_minor_release",
              config_get_bool("autoupdate_on_minor_release") ? "true" : "false");
    ic_printf("%-32s %s\n", "autoupdate_on_patch_release",
              config_get_bool("autoupdate_on_patch_release") ? "true" : "false");
    ic_printf("%-32s %s\n", "autoupdate_when_inactive",
              config_get_bool("autoupdate_when_inactive") ? "true" : "false");
    ic_printf("%-32s %d\n", "autoupdate_inactive_delay",
              config_get_int("autoupdate_inactive_delay"));

    //ic_printf("module settings:\n");
    cfg_t *modules = cfg_getsec(cfg, "modules");
    if (modules != NULL) {
        unsigned int enabled = cfg_size(modules, "enabled");

        for (unsigned int i = 0; i < enabled; i++)
            ic_printf("    %d %s\n", i, cfg_getnstr(modules, "enabled", i));
    }
}

static void validate_settings(void)
{
    long target = config_get_int("cod_version_target");
    if (target != 0 && target != 1 && target != 5)
        config_set_int("cod_version_target", 0);

    long delay = config_get_int("autoupdate_inactive_delay");
    if (delay < 60)
        config_set_int("autoupdate_inactive_delay", 120);
    else if (delay > 1440)
        config_set_int("autoupdate_inactive_delay", 1440);
}

IC_PUBLIC
bool config_get_bool(const char *name)
{
    return (bool) cfg_getbool(cfg, name);
}

IC_PUBLIC
long config_get_int(const char *name)
{
    return cfg_getint(cfg, name);
}

IC_PUBLIC
double config_get_float(const char *name)
{
    return cfg_getfloat(cfg, name);
}

IC_PUBLIC
char *config_get_string(const char *name)
{
    return cfg_getstr(cfg, name);
}

IC_PUBLIC
void config_set_bool(const char *name, bool value)
{
    cfg_setbool(cfg, name, value);
}

IC_PUBLIC
void config_set_int(const char *name, long value)
{
    cfg_setint(cfg, name, value);
}

IC_PUBLIC
void config_set_float(const char *name, double value)
{
    cfg_setfloat(cfg, name, value);
}

IC_PUBLIC
void config_set_string(const char *name, const char *value)
{
    cfg_setstr(cfg, name, value);
}

IC_PUBLIC
void config_init(void)
{
    struct stat f;
    if (stat("iocod.conf", &f) != 0)
        write_default_config();

    cfg = cfg_init(opts, CFGF_IGNORE_UNKNOWN | CFGF_NOCASE);

    int ret = cfg_parse(cfg, "iocod.conf");
    if (ret == CFG_FILE_ERROR)
        ic_error(_("File error with 'iocod.conf'\n"));
    else if (ret == CFG_PARSE_ERROR)
        ic_error(_("Error parsing 'iocod.conf'\n"));

    validate_settings();
    print_values();
}

IC_PUBLIC
void config_shutdown(void)
{
    cfg_free(cfg);
}
