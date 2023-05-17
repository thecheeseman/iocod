// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "main.hpp"
#include <iostream>

IOCOD_NODISCARD int _iocod_main(int argc, char* argv[], char* envp[]) noexcept
{
    std::cerr << "hello there\n";
    return 0;
}
