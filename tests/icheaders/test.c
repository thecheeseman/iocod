#include "iocod.h"

#include <stdio.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdlib.h>

int main(UNUSED int argc, UNUSED char *argv[])
{  
    IC_ASSERT(1 == 1);
    IC_ASSERT(true);

    printf("%d\n", IC_VERSION);
    printf("%s\n", IC_VERSION_STRING);
    printf("%s\n", IC_PLATFORM_COMPILER);

    IC_STATIC_ASSERT(1 == 1, "math doesn't work");
    IC_STATIC_ASSERT(true, "the world is ending");

    int *i = ic_malloc(sizeof(int));
    *i = 52;
    IC_ASSERT(*i == 52);
    ic_free(i);

    void *p = ic_malloc(0);
    ic_free(p);

    char *c = ic_calloc(1, sizeof(char));
    IC_ASSERT(*c == '\0');
    ic_free(c);

    void *ptr = ic_malloc(1024);
    void *ptr2 = ic_realloc(ptr, 2048);
    ptr = ptr2;
    ic_free(ptr);

    return 0;
}
