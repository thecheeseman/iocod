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
    g_commandSystem->Initialize();
    g_system->AddConsoleCommands();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        std::string input = g_system->GetConsoleInput();
        if (input.empty())
            continue;

        g_commandSystem->ExecuteCommandText(input);
    }

    g_commandSystem->Shutdown();
    g_system->Shutdown();
    return 0;
}
