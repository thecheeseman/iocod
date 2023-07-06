// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Engine.h>
#include <Platform/Console.h>

#include "Platform/Windows/PlatformWindows.h"

namespace iocod {

// --------------------------------
// Engine::Initialize
// --------------------------------
void Engine::Initialize(const int argc, char* argv[])
{
    m_platform.Initialize("");
    Console::Initialize();

    //m_platform.Print("Hello, world!\n");

    //m_platform.ShowConsole(ConsoleVisLevel::Hidden, false);
}

// --------------------------------
// Engine::Shutdown
// --------------------------------
void Engine::Shutdown()
{
    Console::Shutdown();
    m_platform.Shutdown();
}

// --------------------------------
// Engine::RunFrame
// --------------------------------
void Engine::RunFrame()
{
    // do stuff
    m_platform.PumpEvents();
    Console::Flush();
}

} // namespace iocod
