#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

enum {
    ERR_OK,
    ERR_NULL_PATH,
    ERR_NULL_HANDLE,
    ERR_NULL_FUNCTION
};

static char *errtext[] = {
    NULL,
    "NULL path",
    "NULL handle",
    "NULL function name"
};

static int localerr = -1;

IC_PUBLIC
char *sys_library_error(void)
{
    // handle local error codes first
    if (localerr != -1) {
        int errcode = localerr;
        localerr = -1;

        return errtext[errcode];
    }

    // system error codes
    #ifdef IC_PLATFORM_WINDOWS
    DWORD err = GetLastError();
    static char msg[1024];

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  err,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  msg,
                  sizeof(msg),
                  NULL);

    return msg;
    #else
    return dlerror();
    #endif
}

IC_PUBLIC
bool sys_library_load(const char *path, void **handle)
{
    *handle = NULL;

    if (path == NULL) {
        localerr = ERR_NULL_PATH;
        return false;
    }

    // append system extension if not provided
    char newpath[MAX_PATH];
    strncpyz(newpath, path, sizeof(newpath));
    ic_append_extension(newpath, sizeof(newpath), "." IC_PLATFORM_DLL);

    // load library
    #ifdef IC_PLATFORM_WINDOWS
    *handle = (void *) LoadLibrary(newpath);
    #else
    *handle = dlopen(newpath, RTLD_NOW);
    #endif

    return (*handle != NULL);
}

IC_PUBLIC
bool sys_library_close(void *handle)
{
    #ifdef IC_PLATFORM_WINDOWS
    return FreeLibrary((HMODULE) handle);
    #else
    return (dlclose(handle) == 0);
    #endif
}

IC_PUBLIC
bool sys_library_load_symbol(void *handle, const char *fn, void **symbol)
{
    *symbol = NULL;

    if (handle == NULL) {
        localerr = ERR_NULL_HANDLE;
        return false;
    }

    if (fn == NULL) {
        localerr = ERR_NULL_FUNCTION;
        return false;
    }

    #ifdef IC_PLATFORM_WINDOWS
    *symbol = (void *) GetProcAddress((HMODULE) handle, fn);
    #else
    *symbol = dlsym(handle, fn);
    #endif

    return (*symbol != NULL);
}
