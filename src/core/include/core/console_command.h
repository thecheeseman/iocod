// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CONSOLE_COMMAND_H
#define CORE_CONSOLE_COMMAND_H

#include <core/types.h>
#include <vector>

namespace iocod {

/// @brief Interface for console commands.
class IConsoleCommand {
public:
    virtual ~IConsoleCommand() = default;

    virtual void Execute(std::vector<String> args) = 0;
};

} // namespace iocod

#endif // CORE_CONSOLE_COMMAND_H
