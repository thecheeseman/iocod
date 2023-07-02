// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Engine.h>
#include <Platform/Console.h>

namespace iocod {

// --------------------------------
// Engine::Engine
// --------------------------------
Engine::Engine(StartupArgs args) :
    m_platform(Platform::GetInstance()),
    m_args(std::move(args))
{
    m_platform.Initialize(m_args.appName);
    Console::Initialize();

    m_platform.Print("Hello, world!\n");

    //m_platform.ShowConsole(ConsoleVisLevel::Hidden, false);
}

// --------------------------------
// Engine::~Engine
// --------------------------------
Engine::~Engine()
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
