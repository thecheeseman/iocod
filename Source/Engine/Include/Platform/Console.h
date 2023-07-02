// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_PLATFORM_CONSOLE_H
#define ENGINE_PLATFORM_CONSOLE_H

#include <Core/String.h>

namespace iocod::Console {

enum class VisLevel {
    Hidden,
    Normal,
    Minimized
};

void Initialize();
void Shutdown();
bool Initialized();
String GetInput();
void Clear();
void Show(VisLevel level, bool exitOnClose);
void Print(const String& message);
void Flush();
void DebugPrint(const String& message);
void DisplayError(const String& message);
void WaitForQuit();

} // namespace iocod::Console

#endif // ENGINE_PLATFORM_CONSOLE_H
