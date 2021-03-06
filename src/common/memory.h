#ifndef COMMON_MEMORY_H
#define COMMON_MEMORY_H

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>

int com_memcmp(const void *str1, const void *str2, size_t n);
void *com_memcpy(void *dest, const void *src, size_t n);
void *com_memmove(void *dest, const void *src, size_t n);
void *com_memset(void *str, int c, size_t n);
void *z_malloc(size_t size);
void z_free(void *p);

#endif /* COMMON_MEMORY_H */
