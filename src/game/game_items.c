#include "game_local.h"
#include "stringlib.h"
#include "va.h"

static bool item_registered[MAX_ITEMS];

void clear_registered_items(void)
{
    memset(item_registered, 0, sizeof(item_registered));

    item_registered[0] = true;
}

void register_item(int num, int other)
{
    int i;
    char *name;
    struct weapon *weapon;
    struct gitem *item;

    if (item_registered[num])
        return;

    item = &bg_item_list[num];

    if (!level.initializing) {
        name = item->display_name;
        if (name == NULL || *name == '\0') {
            if (num <= get_num_weapons()) {
                weapon = get_info_for_weapon(num);
                name = weapon->name;
            }
        }

        if (name == NULL || *name == '\0')
            name = "<<unknown>>";

        scr_error(va("game tried to register the item '%s' after " \
                     "initialization finished", name));
    }

    item_registered[num] = true;

    i = num;
    if (item->type == IT_WEAPON) {
        do {
            item_registered[i] = true;
            weapon = get_info_for_weapon(i);

            model_index(weapon->world_model);
            model_index(weapon->projectile_model);

            i = weapon->alt_weapon_index;
        } while (i != num);
    } else {
        if (item->world_model[0] != NULL)
            model_index(item->world_model[0]);
        if (item->world_model[1] != NULL)
            model_index(item->world_model[1]);
    }

    if (other)
        level.field_10768 = true;
}
