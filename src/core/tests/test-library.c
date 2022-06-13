#include "ic_test.h"

#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

/**
 * @brief Get last library error, if applicable. Clears last error.
 * @return NULL-terminated string containing error message, otherwise NULL
*/
char *syslib_error(void)
{
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

/**
 * @brief Load a system library.
 * @param[in]  path   file path to library module
 * @param[out] handle handle to loaded library module, NULL if failed
 * @return true if success, false otherwise
*/
bool syslib_load(const char *path, void **handle)
{
    *handle = NULL;

    if (path == NULL)
        return false;

    #ifdef IC_PLATFORM_WINDOWS
    *handle = (void *) LoadLibrary(path);
    #else
    *handle = dlopen(path, RTLD_NOW);
    #endif

    return (*handle != NULL);
}

/**
 * @brief Close a system library.
 * @param[in] handle loaded library module
 * @return true if success, false otherwise
*/
bool syslib_close(void *handle)
{
    #ifdef IC_PLATFORM_WINDOWS
    return FreeLibrary((HMODULE) handle);
    #else
    return (dlclose(handle) == 0);
    #endif
}

/**
 * @brief Load a given @p symbol in a library module @p handle
 * 
 * @note MSVC will throw warnings if you do not cast @p symbol to (void *)
 * 
 * @param[in] handle  loaded library module
 * @param[in] fn      name of symbol to load
 * @param[out] symbol handle to symbol, NULL if failed
 * @return true if success, false otherwise
*/
bool syslib_symbol(void *handle, const char *fn, void **symbol)
{
    *symbol = NULL;

    if (handle == NULL)
        return false;

    #ifdef IC_PLATFORM_WINDOWS
    *symbol = (void *) GetProcAddress((HMODULE) handle, fn);
    #else
    *symbol = dlsym(handle, fn);
    #endif

    return (*symbol != NULL);
}

int TEST_MAIN()
{
    void *handle;
    if (!syslib_load("user32.dll", &handle)) {
        fprintf(stderr, "error: %s\n", syslib_error());
        return 1;
    }

    int (*messagebox)(HWND, LPCSTR, LPCSTR, UINT);
    if (!syslib_symbol(handle, "MessageBoxA", (void *) &messagebox)) {
        fprintf(stderr, "error: %s\n", syslib_error());
        return 1;
    }

    messagebox(NULL, "general kenobi", "hello there", MB_ICONWARNING);

    syslib_close(handle);

    return 0;
}
