#include "../icheaders/iocod.h"

#include <stdio.h>

#ifdef IC_PLATFORM_WINDOWS
#include "../../external/dlfcn-win32/dlfcn.h"
#define TEST_DLL "game.dll"
#else
#include <dlfcn.h>
#define TEST_DLL "game.so"
#endif

int main(void)
{
    void *handle = dlopen(TEST_DLL, RTLD_NOW);

    if (handle == NULL) {
        printf("failed: %s\n", dlerror());
        return 0;
    }

    void (*dllentry)(intptr_t(*syscallptr)(intptr_t, ...)) =
        dlsym(handle, "dllEntry");

    intptr_t (*entry)(intptr_t, ...) =
        dlsym(handle, "vmMain");

    if (entry == NULL || dllentry == NULL) {
        printf("failed dlsym\n");

        dlclose(handle);
        char *err = dlerror();
        if (err != NULL)
            printf("%s\n", err);

        return 0;
    }

    printf("success\n");

    dlclose(handle);

    return 0;
}
