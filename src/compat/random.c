#include "compat/random.h"

#include <stdlib.h>

#if defined PLATFORM_WINDOWS
long int random(void)
{
    return (rand() % RAND_MAX);
}
#endif
