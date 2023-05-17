// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <chrono>

#include <Console.hpp>

// --------------------------------
// main
// --------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    using namespace iocod;

    Console& console = Console::GetInstance();
    auto [result, errormsg] = console.Initialize();
    if (!result) {
        fprintf(stderr, "Error initializing console: %s\n", errormsg.c_str());
        return 1;
    }

    console.SetTitle("hello there");
    console.Print("hello there\n");

    console.Warn("this is a warning\n");
    console.Error("this is an error message\n");

    for (int i = 0; i < 10; ++i)
        console.Print("This is a test with ^" + std::to_string(i) + "color strings\n");

    while (true) {
        std::string input = console.GetInput();
        if (input.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }

        if (input == "quit")
            break;
    }

    console.Shutdown();

    return 0;
}
