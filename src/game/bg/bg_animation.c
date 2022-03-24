#include "game/game_local.h"

#if 0
struct anim_string_item {
    char *string;
    int hash;
};

#define STRUCT(x) static struct anim_string_item x[] = {
#define ENTRY(x) { #x, -1 },
#define END() { NULL, -1 } };

STRUCT(anim_state_str)
    ENTRY(RELAXED)
    ENTRY(QUERY)
    ENTRY(ALERT)
    ENTRY(COMBAT)
END()

STRUCT(anim_move_types_str)
    ENTRY(**UNUSED**)
    ENTRY(IDLE)
    ENTRY(IDLECR)
    ENTRY(IDLEPRONE)
    ENTRY(WALK)
    ENTRY(WALKBK)
    ENTRY(WALKCR)
    ENTRY(WALKCRBK)
    ENTRY(WALKPRONE)
    ENTRY(WALKPRONEBK)
    ENTRY(RUN)
    ENTRY(RUNBK)
    ENTRY(RUNCR)
    ENTRY(RUNCRBK)
    ENTRY(TURNRIGHT)
    ENTRY(TURNLEFT)
    ENTRY(CLIMBUP)
    ENTRY(CLIMBDOWN)
END()

STRUCT(anim_event_types_str)
    ENTRY(PAIN)
    ENTRY(DEATH)
    ENTRY(FIREWEAPON)
    ENTRY(JUMP)
    ENTRY(JUMPBK)
    ENTRY(LAND)
    ENTRY(DROPWEAPON)
    ENTRY(RAISEWEAPON)
    ENTRY(CLIMBMOUNT)
    ENTRY(CLIMBDISMOUNT)
    ENTRY(RELOAD)
    ENTRY(CROUCH_TO_PRONE)
    ENTRY(PRONE_TO_CROUCH)
    ENTRY(MELEEATTACK)
END()

STRUCT(anim_body_parts_str)
    ENTRY(**UNUSED**)
    ENTRY(LEGS)
    ENTRY(TORSO)
    ENTRY(BOTH)
END()

STRUCT(anim_condition_positions_str)
    ENTRY(**UNUSED**)
    ENTRY(BEHIND)
    ENTRY(INFRONT)
    ENTRY(RIGHT)
    ENTRY(LEFT)
END()

STRUCT(anim_condition_mounted_str)
    ENTRY(**UNUSED**)
    ENTRY(MG42)
END()

STRUCT(anim_weapon_class_str)
    ENTRY(RIFLE)
    ENTRY(MG)
    ENTRY(SMG)
    ENTRY(PISTOL)
    ENTRY(GRENADE)
    ENTRY(ROCKETLAUNCHER)
    ENTRY(TURRET)
    ENTRY(NON-PLAYER)
END()

STRUCT(anim_weapon_position_str)
    ENTRY(HIP)
    ENTRY(ADS)
END()

STRUCT(anim_strafe_state_str)
    ENTRY(NOT)
    ENTRY(LEFT)
    ENTRY(RIGHT)
END()

STRUCT(anim_conditions_str)
    ENTRY(WEAPONS)
    ENTRY(WEAPONCLASS)
    ENTRY(MOUNTED)
    ENTRY(MOVETYPE)
    ENTRY(UNDERHAND)
    ENTRY(CROUCHING)
    ENTRY(FIRING)
    ENTRY(WEAPON_POSITION)
    ENTRY(STRAFING)
END()

#undef STRUCT
#undef ENTRY
#undef END

#endif
