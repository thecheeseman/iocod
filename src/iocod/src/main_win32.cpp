// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <chrono>
#include <core/system.h>
#include <windows.h>

// --------------------------------
// WinMain
// --------------------------------
int WINAPI WinMain(const HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                   [[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
    using namespace iocod;

    sys->Initialize();

    sys->DebugPrint("debug print example\n");
    sys->Print("hello there\n");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        sys->PumpEvents();

        if (sys->GetConsoleInput() == "quit") {
            break;
        }
    }

    sys->Shutdown();

    return 0;
}
