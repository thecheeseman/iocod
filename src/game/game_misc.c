#include "game_local.h"
#include "stringlib.h"

bool get_hint_string_index(int *hint, char *string)
{
    int i;
    char s[MAX_STRING_CHARS];

    for (i = 0; i < MAX_CONFIG_STRINGS; i++) {
        trap_get_config_string(CS_HINT_STRINGS + i, s, sizeof(s));
        if (!s[0])
            break;

        if (strcmp(s, string) == 0) {
            *hint = i;
            return true;
        }
    }

    if (i == MAX_CONFIG_STRINGS) {
        *hint = -1;
        return false;
    }

    trap_set_config_string(CS_HINT_STRINGS + i, string);
    *hint = i;
    return true;
}

int INCOMPLETE model_index(const char *model)
{
    int i;
    const char *str;

    if (model == NULL || *model == '\0')
        return 0;

    for (i = 1; i < MAX_MODELS; i++) {
        str = trap_get_config_string_const(CS_MODELS + i);
        if (str == NULL || *str == '\0')
            break;

        if (strcasecmp(model, str) == 0)
            return i;
    }

    if (!level.initializing)
        scr_error(va("model '%s' not precached", model));

    if (i == MAX_MODELS)
        g_error("model_index: overflow");

    // xmodel = trap_xmodel_get(model);
    // xmodels[i] = xmodel;
    trap_set_config_string(CS_MODELS + i, model);
    return i;
}
