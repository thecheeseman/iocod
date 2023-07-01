// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Application.h>
#include <chrono>

namespace iocod {

Application::Application(const String& appName, const std::size_t argc, const String& args)
    : m_engine(StartupArgs{appName, argc, args})
{
    
}

Application::~Application()
{
    // TODO:
}

void Application::Run()
{
    while (!m_engine.QuitRequested()) {
        m_engine.RunFrame();
    }
}

} // namespace iocod
