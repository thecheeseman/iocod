// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/Assert.h>
#include <stdio.h>

void _iocod_report_assertion_failure(const char* filename, int line,
                                     const char* function, const char* message)
{
    fprintf(stderr, "%s:%d internal check failed at %s: %s\n", filename, line,
            function, message);
    fflush(stderr);
}