// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <chrono>
#include <Core/CommandSystem.h>
#include <Core/System.h>
#include <windows.h>

// --------------------------------
// WinMain
// --------------------------------
int WINAPI WinMain(const HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                   [[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
    using namespace iocod;

    #if 0

    sys->Initialize(hInstance);
    commandSystem->Initialize();

    sys->DebugPrint("debug print example\n");
    sys->Print("hello there\n");

    size_t times = 0;
    auto test = [&times] {
        times++;
        for (int i = 0; i < 10000; ++i) {
            sys->Print(fmt::format("{}: {}\n", times, i));
        }
    };

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        sys->PumpEvents();

        if (auto message = sys->GetConsoleInput(); !message.empty()) {
            commandSystem->AddCommandText(message);
        }

        commandSystem->ExecuteCommandBuffer();
        sys->FlushConsole();
    }

    sys->Shutdown();

    #endif

    return 0;
}
