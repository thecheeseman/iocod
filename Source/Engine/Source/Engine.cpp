// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Engine.h>

namespace iocod {

Engine::Engine(StartupArgs args) :
    m_platform(Platform::GetInstance()),
    m_args(std::move(args))
{
    m_platform.Initialize(m_args.appName);
}

Engine::~Engine()
{
    m_platform.Shutdown();
}

void Engine::RunFrame()
{
    // do stuff
}

} // namespace iocod
