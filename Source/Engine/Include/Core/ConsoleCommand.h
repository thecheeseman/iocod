// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CONSOLE_COMMAND_H
#define CORE_CONSOLE_COMMAND_H

#include <Core/Types.h>
#include <Core/ClassNonCopyable.h>
#include <vector>

namespace iocod {

/// @brief Interface for console commands.
class IConsoleCommand {
public:
    IConsoleCommand() = default;
    virtual ~IConsoleCommand() = default;
    CLASS_NON_COPYABLE(IConsoleCommand)

    virtual void Execute(std::vector<String> args) = 0;
};

} // namespace iocod

#endif // CORE_CONSOLE_COMMAND_H
