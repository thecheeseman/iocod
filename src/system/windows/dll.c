#include "system/windows/local.h"

void *sys_load_dll(const char *name, char *fqpath,
                   intptr_t(**entrypoint)(intptr_t, ...),
                   intptr_t(*systemcalls)(intptr_t, ...))
{
    return NULL;
}

void sys_unload_dll(void *dllhandle)
{

}
