// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

#include <Engine.h>
#include <Core/String.h>

namespace iocod {

class IOCOD_API Application {
public:
    CLASS_NON_COPYABLE(Application)

    Application(const String& appName, std::size_t argc, const String& args);
    ~Application();

    void Run();

private:
    Engine m_engine;
};

} // namespace iocod

#endif // ENGINE_APPLICATION_H
