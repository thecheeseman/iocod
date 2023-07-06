// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>
#include <Assert.h>
#include <Core/Log.h>

namespace iocod::detail {

void ReportAssertionFailure(const char* filename, const int line, const char* function,
                            const char* message)
{
    LogFatal("[%s:%d] Assertion failed in %s(): %s\n", filename, line, function, message);
}

} // namespace iocod::detail
