// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Application.h>
#include <chrono>

namespace iocod {

static bool g_initializedGlobalState = false;

// --------------------------------
// ApplicationGlobalState::ApplicationGlobalState
// --------------------------------
ApplicationGlobalState::ApplicationGlobalState()
{
    Assert(!g_initializedGlobalState);

    Memory::Initialize();

    m_initialized = true;
    g_initializedGlobalState = true;
}

ApplicationGlobalState::~ApplicationGlobalState()
{
    Assert(m_initialized);
    Assert(g_initializedGlobalState);

    m_initialized = false;
    g_initializedGlobalState = false;

    Memory::Shutdown();
}

// --------------------------------
// Application::Initialize
// --------------------------------
void Application::Initialize(int argc, char* argv[])
{
    m_engine.Initialize(argc, argv);
}

// --------------------------------
// Application::Run
// --------------------------------
void Application::Run()
{
    while (!m_engine.QuitRequested()) {
        m_engine.RunFrame();
    }
}

// --------------------------------
// Application::Shutdown
// --------------------------------
void Application::Shutdown()
{
    m_engine.Shutdown();
}

} // namespace iocod
