#ifndef INCLUDE_TYPES_NULL_H
#define INCLUDE_TYPES_NULL_H

#if defined(HAVE_CONFIG_H) && defined(HAVE_STDDEF_H)
#include <stddef.h>
#else
#ifndef NULL
#define NULL ((void *) 0)
#endif
#endif

#endif /* INCLUDE_TYPES_NULL_H */
