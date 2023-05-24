// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/command_system.h>
#include <core/system.h>

#include <chrono>

// --------------------------------
// main
// --------------------------------
int main(int argc, char** argv, char** envp)
{
    using namespace iocod;

    g_system->Initialize();
    g_command_system->Initialize();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        std::string input = g_system->GetConsoleInput();
        if (input.empty())
            continue;

        g_command_system->ExecuteCommandText(input);
    }

    g_command_system->Shutdown();
    g_system->Shutdown();
    return 0;
}
