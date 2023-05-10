// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Console.hpp>

int main(int argc, char* argv[])
{
    Console& console = Console::Get();
    if (!console.Initialize())
        return 1;

    console.Print("Hello there\n");
    console.Print("asdf");

    console.Shutdown();
    return 0;
}
