// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Application.h>
#include <Core/String.h>

// --------------------------------
// main
// --------------------------------
int main(const int argc, const char* argv[])
{
    using namespace iocod;

    // TODO: do this better
    String args;
    for (int i = 0; i < argc; i++) {
        args += String{argv[i]};
        args += " ";
    }

    Application app("iocod", argc, args);
    app.Run();

    return 0;
}
