// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifdef IOCOD_OS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#include <core/assert.h>
#include <core/platform.h>

namespace iocod::detail {

void ReportAssertionFailure(const char* filename, const int line, const char* function,
                            const char* message)
{
    static char buffer[4096]{};

    (void) snprintf(buffer, sizeof(buffer), "[%s:%d] Assertion failed in %s(): %s\n", filename,
                    line, function, message);
    (void) fputs(buffer, stderr);

#ifdef IOCOD_OS_WINDOWS
    if (::IsDebuggerPresent())
        OutputDebugStringA(buffer);
#endif
}

} // namespace iocod::detail
