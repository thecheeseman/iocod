// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Application.h>
#include <Core/String.h>
#include <windows.h>

// --------------------------------
// WinMain
// --------------------------------
int WINAPI WinMain(const HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
                   [[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
    using namespace iocod;

    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv) {
        wprintf(L"CommandLineToArgvW failed\n");
        return 0;
    }

    // TODO: do this better
    String args;
    for (int i = 0; i < argc; i++) {
        args += String{argv[i]};
        args += " ";
    }

    LocalFree(argv);

    Application app("iocod", argc, args);
    app.Run();

    return 0;
}
