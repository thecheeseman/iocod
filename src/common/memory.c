#include "iocod.h"

#include <stdio.h>
#include <stdlib.h>

#include "common/memory.h"
#include "strings/stringlib.h"

int com_memcmp(const void *str1, const void *str2, size_t n)
{
    return memcmp(str1, str2, n);
}

void *com_memcpy(void *dest, const void *src, size_t n)
{
    return memcpy(dest, src, n);
}

void *com_memmove(void *dest, const void *src, size_t n)
{
    return memmove(dest, src, n);
}

void *com_memset(void *str, int c, size_t n)
{
    return memset(str, c, n);
}

static void out_of_memory_error(void)
{
    fprintf(stderr, "OUT OF MEMORY! ABORTING!!!\n");
    exit(-1);
}

void *z_malloc(size_t size)
{
    void *p = malloc(size);
    if (p == NULL)
        out_of_memory_error();

    memset(p, 0, size);
    return p;
}

void z_free(void *p)
{
    if (p != NULL)
        free(p);
}
