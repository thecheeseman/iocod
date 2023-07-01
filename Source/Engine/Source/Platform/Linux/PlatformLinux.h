// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_PLATFORM_LINUX_PLATFORM_LINUX_H
#define ENGINE_PLATFORM_LINUX_PLATFORM_LINUX_H

#include <Platform/Platform.h>

#include <dlfcn.h>

namespace iocod {

class PlatformLinux final : public Platform {
public:
    void Initialize() override;
    void Shutdown() override;

    String GetLastErrorAsString() override;
    void DisplayFatalErrorAndExit(const String& errorMessage) override;

    void* LibraryOpen(const String& path) override;
    void* LibraryLoadSymbol(void* library, const String& symbolName) override;
    void LibraryClose(void* library) override;

private:
};

} // namespace iocod

#endif // ENGINE_PLATFORM_LINUX_PLATFORM_LINUX_H
