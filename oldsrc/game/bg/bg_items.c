#include "../game_local.h"

struct gitem bg_item_list[] = {
    #define NULL_ITEM() \
        { \
            NULL, \
            NULL, \
            { \
                NULL, \
                NULL \
            }, \
            NULL, \
            NULL, \
            NULL, \
            0, \
            0, \
            0, \
            0, \
            0 \
        },
    #define EMPTY_ITEM(n) \
        { \
            "emptyitem_\"" #n "\"", \
            "", \
            { \
                "", \
                "" \
            }, \
            "", \
            "", \
            "", \
            0, \
            0, \
            0, \
            0, \
            0 \
        },

    NULL_ITEM()

    // 64 blank weapons
    // these are filled in by the weapons/mp files
    EMPTY_ITEM(w01)
    EMPTY_ITEM(w02)
    EMPTY_ITEM(w03)
    EMPTY_ITEM(w04)
    EMPTY_ITEM(w05)
    EMPTY_ITEM(w06)
    EMPTY_ITEM(w07)
    EMPTY_ITEM(w08)
    EMPTY_ITEM(w09)
    EMPTY_ITEM(w10)
    EMPTY_ITEM(w11)
    EMPTY_ITEM(w12)
    EMPTY_ITEM(w13)
    EMPTY_ITEM(w14)
    EMPTY_ITEM(w15)
    EMPTY_ITEM(w16)
    EMPTY_ITEM(w17)
    EMPTY_ITEM(w18)
    EMPTY_ITEM(w19)
    EMPTY_ITEM(w20)
    EMPTY_ITEM(w21)
    EMPTY_ITEM(w22)
    EMPTY_ITEM(w23)
    EMPTY_ITEM(w24)
    EMPTY_ITEM(w25)
    EMPTY_ITEM(w26)
    EMPTY_ITEM(w27)
    EMPTY_ITEM(w28)
    EMPTY_ITEM(w29)
    EMPTY_ITEM(w30)
    EMPTY_ITEM(w31)
    EMPTY_ITEM(w32)
    EMPTY_ITEM(w33)
    EMPTY_ITEM(w34)
    EMPTY_ITEM(w35)
    EMPTY_ITEM(w36)
    EMPTY_ITEM(w37)
    EMPTY_ITEM(w38)
    EMPTY_ITEM(w39)
    EMPTY_ITEM(w40)
    EMPTY_ITEM(w41)
    EMPTY_ITEM(w42)
    EMPTY_ITEM(w43)
    EMPTY_ITEM(w44)
    EMPTY_ITEM(w45)
    EMPTY_ITEM(w46)
    EMPTY_ITEM(w47)
    EMPTY_ITEM(w48)
    EMPTY_ITEM(w49)
    EMPTY_ITEM(w50)
    EMPTY_ITEM(w51)
    EMPTY_ITEM(w52)
    EMPTY_ITEM(w53)
    EMPTY_ITEM(w54)
    EMPTY_ITEM(w55)
    EMPTY_ITEM(w56)
    EMPTY_ITEM(w57)
    EMPTY_ITEM(w58)
    EMPTY_ITEM(w59)
    EMPTY_ITEM(w60)
    EMPTY_ITEM(w61)
    EMPTY_ITEM(w62)
    EMPTY_ITEM(w63)
    EMPTY_ITEM(w64)
    // 64 empty weapon slots

    {
        "item_ammo_stielhandgranate_open",
        "grenade_pickup",
        {
            "xmodel/ammo_stielhandgranate1",
            0
        },
        "gfx/icons/hud@stielhandgrenate",
        "gfx/icons/hud@stielhandgrenate",
        "Stielhandgranate_mp Ammo Open",
        10,
        IT_AMMO,
        -1,
        -1,
        -1
    },
    {
        "item_ammo_stielhandgranate_closed",
        "grenade_pickup",
        {
            "xmodel/ammo_stielhandgranate2",
            0
        },
        "gfx/icons/hud@stielhandgrenate",
        "gfx/icons/hud@stielhandgrenate",
        "Stielhandgranate_mp Ammo Closed",
        10,
        IT_AMMO,
        -1,
        -1,
        -1
    },

    {
        "item_health_small",
        "health_pickup_small",
        {
            "xmodel/health_small",
            0
        },
        "icons/iconh_small",
        NULL,
        "Small Health",
        10,
        IT_HEALTH,
        0,
        0,
        0
    },
    {
        "item_health",
        "health_pickup_medium",
        {
            "xmodel/health_medium",
            0
        },
        "icons/iconh_med",
        NULL,
        "Med Health",
        25,
        IT_HEALTH,
        0,
        0,
        0
    },
    {
        "item_health_large",
        "health_pickup_large",
        {
            "xmodel/health_large",
            0
        },
        "icons/iconh_large",
        NULL,
        "Large Health",
        50,
        IT_HEALTH,
        0,
        0,
        0
    },

    #undef NULL_ITEM
    #undef EMPTY_ITEM

    { NULL }
};
