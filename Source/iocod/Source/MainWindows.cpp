// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Application.h>
#include <clocale>
#include <windows.h>
#include <Core/String.h>
#include <tchar.h>

// --------------------------------
// WinMain
// --------------------------------
int APIENTRY _tWinMain(const HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
    UNUSED(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

    iocod::ApplicationGlobalState globalState;
    iocod::Application app;
    app.Initialize(__argc, __argv);
    app.Run();
    app.Shutdown();

    return 0;
}
