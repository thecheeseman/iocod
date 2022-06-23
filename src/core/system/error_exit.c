#include "iocod.h"

#ifdef IC_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>

// from MDSN
IC_PUBLIC
void sys_handle_error_exit(const char *function)
{
    // Retrieve the system error message for the last-error code
    LPVOID lpMsgBuf = NULL;
    LPVOID lpDisplayBuf = NULL;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process
    lpDisplayBuf = (LPVOID) LocalAlloc(LMEM_ZEROINIT,
                                       (lstrlen((LPCTSTR) lpMsgBuf) +
                                        lstrlen((LPCTSTR) function) + 40) *
                                       sizeof(TCHAR));
    StringCchPrintf((LPTSTR) lpDisplayBuf,
                    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                    TEXT("%s failed with error %d: %s"),
                    function, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR) lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

    // ExitProcess(dw);
    sys_exit(IC_TERMINATE);
}
#else
IC_PUBLIC
void sys_handle_error_exit(const char *function)
{
    UNUSED_PARAM(function);
    
    sys_exit(IC_TERMINATE);
}
#endif
