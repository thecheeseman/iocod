// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <Core/String.h>
#include <Platform/Platform.h>

namespace iocod {

class IOCOD_API Engine final {
public:
    CLASS_NON_COPYABLE(Engine)

    Engine() :
        m_platform(Platform::GetInstance()) {}

    ~Engine() = default;

    void Initialize(int argc, char* argv[]);
    void Shutdown();

    void RunFrame();
    [[nodiscard]] bool QuitRequested() const { return m_quitRequested; }

private:
    bool m_quitRequested = false;
    Platform& m_platform;
};

} // namespace iocod

#endif // ENGINE_ENGINE_H
