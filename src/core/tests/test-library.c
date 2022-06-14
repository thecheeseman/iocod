#include "ic_test.h"

#include "iocod.h"

int TEST_MAIN()
{
    void *handle;
    bool result;

    // verify handling of NULL paths
    result = sys_library_load(NULL, &handle);
    IC_ASSERT(result == false);
    IC_ASSERT(strcmp(sys_library_error(), "NULL path") == 0);

    // verify handling of NULL handles
    void *symbol;
    result = sys_library_load_symbol(handle, NULL, &symbol);
    IC_ASSERT(result == false);
    IC_ASSERT(strcmp(sys_library_error(), "NULL handle") == 0);

    // verify handling of NULL function names
    handle = (void *) 1;
    result = sys_library_load_symbol(handle, NULL, &symbol);
    IC_ASSERT(result == false);
    IC_ASSERT(strcmp(sys_library_error(), "NULL function name") == 0);

    #ifdef IC_PLATFORM_WINDOWS
    // verify regular path works
    result = sys_library_load("user32.dll", &handle);
    IC_ASSERT(result);
    IC_ASSERT(handle != NULL);

    // verify library close
    result = sys_library_close(handle);
    IC_ASSERT(result);

    // verify automatic system dll extension
    result = sys_library_load("user32", &handle);
    IC_ASSERT(result);
    IC_ASSERT(handle != NULL);

    // verify loading of symbols
    int (*messagebox)(HWND, LPCSTR, LPCSTR, UINT);
    result = sys_library_load_symbol(handle, "MessageBoxA", 
                                     (void *) &messagebox);
    IC_ASSERT(result);
    IC_ASSERT(messagebox != NULL);

    // messagebox(NULL, "test message", "test message", MB_OK);
    #else
    // verify regular path works
    result = sys_library_load("libm-2.31.so", &handle);
    IC_ASSERT(result);
    IC_ASSERT(handle != NULL);

    result = sys_library_close(handle);
    IC_ASSERT(result);

    // verify automatic system so extension
    result = sys_library_load("libm-2.31", &handle);
    IC_ASSERT(result);
    IC_ASSERT(handle != NULL);

    // verify loading of symbols
    double (*_sqrt)(double);
    result = sys_library_load_symbol(handle, "sqrt", (void *) &_sqrt);
    IC_ASSERT(result);
    IC_ASSERT(_sqrt != NULL);
    #endif

    sys_library_close(handle);

    return 0;
}
