// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/assert.h>
#include <core/system.h>

namespace iocod::detail {

void iocod_report_assertion_failure(const char* filename, int line, const char* function,
                                    const char* message)
{
    System->LogErrorNoExit(
        fmt::format("{}:{} internal check failed at {}: {}", filename, line, function, message));
}

} // namespace iocod::detail
