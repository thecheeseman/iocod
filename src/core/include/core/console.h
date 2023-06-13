// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CONSOLE_H
#define CORE_CONSOLE_H

#include <core/types.h>

namespace iocod {

enum class ConsoleVisLevel {
    Hidden,
    Normal,
    Minimized
};

class IConsole {
public:
    virtual ~IConsole() = default;

    virtual void Create(void* handle = nullptr) = 0;
    virtual void Destroy() = 0;

    virtual void PumpEvents() = 0;

    virtual String GetInput() = 0;
    virtual void Show(const ConsoleVisLevel level, const bool quitOnClose) = 0;

    virtual void DisplayError(const String& message) = 0;
};

extern IConsole* console;

} // namespace iocod

#endif // CORE_CONSOLE_H
