#ifndef IOCOD_H
#define IOCOD_H

#if defined HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "build_number.h"
#include "platform.h"

/* from kernel.h */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define FIELD_SIZEOF(t, f) (sizeof(((t*)0)->f))

#define UNUSED(x) (void)(x)

/* utilities for function marking */
#define INCOMPLETE
#define NOT_WORKING

#endif /* IOCOD_H */
