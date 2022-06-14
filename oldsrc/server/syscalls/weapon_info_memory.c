#include <stddef.h>
#include "common/hunk.h"
#include "weapon_info_memory.h"

static bool weapon_info_allocated = false;
static void *weapon_info_memory;

intptr_t get_weapon_info_memory(int size, bool *already_allocated, bool val)
{
    if (size < 0)
        return 0;

    if (weapon_info_memory == NULL) {
        weapon_info_memory = hunk_alloc_low(size);
        weapon_info_allocated = val;

        *already_allocated = false;
    } else {
        *already_allocated = weapon_info_allocated;

        if (!weapon_info_allocated)
            weapon_info_allocated = val;
    }

    return (intptr_t) weapon_info_memory;
}

void free_weapon_info_memory(bool allocated, bool keep_memory)
{
    if (weapon_info_allocated == allocated) {
        if (!keep_memory)
            weapon_info_memory = NULL;

        weapon_info_allocated = false;
    }
}
