// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/command_system.h>
#include <core/file_system.h>
#include <core/system.h>

#include <chrono>

// --------------------------------
// main
// --------------------------------
int main(int argc, char** argv, char** envp)
{
    using namespace iocod;

    System->Initialize();
    FileSystem->Initialize();
    CommandSystem->Initialize();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        std::string input = System->GetConsoleInput();
        if (input.empty())
            continue;

        CommandSystem->ExecuteCommandText(input);
    }

    CommandSystem->Shutdown();
    FileSystem->Shutdown();
    System->Shutdown();
    return 0;
}
