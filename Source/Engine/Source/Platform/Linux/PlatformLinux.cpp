// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PlatformLinux.h"

#include <Core/Log.h>

namespace iocod {

Platform& Platform::GetInstance()
{
    static constinit PlatformLinux s_platform;
    return s_platform;
}

// --------------------------------
// PlatformLinux::GetLastErrorAsString
// --------------------------------
String PlatformLinux::GetLastErrorAsString()
{
    const int error = errno;
    if (error == 0)
        return String{};

    return strerror(error);
}

// --------------------------------
// PlatformLinux::DisplayFatalErrorAndExit
// --------------------------------
[[noreturn]] void PlatformLinux::DisplayFatalErrorAndExit(const String& errorMessage)
{
    fprintf(stderr, "%s\n", errorMessage.c_str());
    exit(1);
}

// --------------------------------
// PlatformLinux::Initialize
// --------------------------------
void PlatformLinux::Initialize()
{
}

// --------------------------------
// PlatformLinux::Shutdown
// --------------------------------
void PlatformLinux::Shutdown() { }

// --------------------------------
// PlatformLinux::LibraryOpen
// --------------------------------
void* PlatformLinux::LibraryOpen(const String& path)
{
    return dlopen(path.c_str());
}

// --------------------------------
// PlatformLinux::LibraryLoadSymbol
// --------------------------------
void* PlatformLinux::LibraryLoadSymbol(void* library, const String& symbolName)
{
    return dlsym(library, symbolName.c_str());
}

// --------------------------------
// PlatformLinux::LibraryClose
// --------------------------------
void PlatformLinux::LibraryClose(void* library)
{
    dlclose(library);
}

} // namespace iocod
