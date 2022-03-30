#ifndef SHARED_WEAPON_INFO_MEM_H
#define SHARED_WEAPON_INFO_MEM_H

#include <stdint.h>
#include "types/bool.h"

intptr_t get_weapon_info_memory(int size, bool *already_allocated, bool val);
void free_weapon_info_memory(bool allocated, bool keep_memory);

#endif /* SHARED_WEAPON_INFO_MEM_H */
