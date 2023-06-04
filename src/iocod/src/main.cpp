// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <chrono>
#include <core/command_system.h>
#include <core/file_system.h>
#include <core/shared_library.h>
#include <core/system.h>

// --------------------------------
// main
// --------------------------------
int main(int argc, char** argv, char** envp)
{
    using namespace iocod;

    sys->Initialize();
    file_system->Initialize();
    command_system->Initialize();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        std::string input = sys->GetConsoleInput();
        if (input.empty())
            continue;

        command_system->ExecuteCommandText(input);
    }

    command_system->Shutdown();
    file_system->Shutdown();
    sys->Shutdown();
    return 0;
}
