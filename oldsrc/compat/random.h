#ifndef COMPAT_RANDOM_H
#define COMPAT_RANDOM_H

#include "iocod.h"

#if defined PLATFORM_WINDOWS
long int random(void);
#endif

#endif /* COMPAT_RANDOM_H */
