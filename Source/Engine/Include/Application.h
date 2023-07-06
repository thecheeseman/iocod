// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include <Engine.h>
#include <Core/String.h>

namespace iocod {

class IOCOD_API ApplicationGlobalState {
public:
    CLASS_NON_COPYABLE(ApplicationGlobalState)

    ApplicationGlobalState();
    ~ApplicationGlobalState();

private:
    bool m_initialized = false;
};

class IOCOD_API Application {
public:
    CLASS_NON_COPYABLE(Application)

    Application() = default;
    ~Application() = default;

    void Initialize(int argc, char* argv[]);
    void Run();
    void Shutdown();

private:
    Engine m_engine;
};

} // namespace iocod

#endif // ENGINE_APPLICATION_H
