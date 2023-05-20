// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/shared_library.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

namespace iocod {

// --------------------------------
// SharedLibrary::SharedLibrary
// --------------------------------
SharedLibrary::SharedLibrary(const std::filesystem::path& library_path) noexcept
{
    Load(library_path);
}

SharedLibrary::~SharedLibrary()
{
    if (Loaded())
        Unload();
}

SharedLibrary::SharedLibrary(SharedLibrary&& other) noexcept
{
    *this = std::move(other);
}

SharedLibrary& SharedLibrary::operator=(SharedLibrary&& other) noexcept
{
    handle = other.handle;
    loaded = other.loaded;
    path = std::move(other.path);

    other.handle = nullptr;
    other.loaded = false;

    return *this;
}

// --------------------------------
// SharedLibrary::Loaded
// --------------------------------
bool SharedLibrary::Loaded() const noexcept
{
    return loaded;
}

// --------------------------------
// SharedLibrary::Load
// --------------------------------
bool SharedLibrary::Load(const std::filesystem::path& library_path) noexcept
{
    if (Loaded())
        Unload();

    path = library_path;
    loaded = false;

#ifdef _WIN32
    // TODO: support wide strings
    handle = LoadLibraryA(library_path.string().c_str());
#else
    handle = dlopen(library_path.c_str(), RTLD_LAZY);
#endif

    if (handle == nullptr) {
        std::string message = "Failed to load library '" + library_path.string() + "': ";

#ifdef _WIN32
        LPSTR buffer = nullptr;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr,
                       GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       reinterpret_cast<LPSTR>(&buffer), 0, nullptr);
        message += buffer;
        LocalFree(buffer);
#else
        message += dlerror();
#endif

        SetLastErrorMessage(message);
    } else {
        loaded = true;
    }

    return loaded;
}

// --------------------------------
// SharedLibrary::Unload
// --------------------------------
void SharedLibrary::Unload() noexcept
{
    if (!Loaded())
        return;

#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(handle));
#else
    dlclose(handle);
#endif

    handle = nullptr;
    loaded = false;
}

// --------------------------------
// SharedLibrary::LoadVoidSymbol
// --------------------------------
void* SharedLibrary::LoadVoidSymbol(const char* symbol) noexcept
{
    if (!Loaded()) {
        SetLastErrorMessage("Tried to load symbol '" + std::string(symbol) +
                            "' from an unloaded library");
        return nullptr;
    }

#ifdef _WIN32
    void* addr = GetProcAddress(static_cast<HMODULE>(handle), symbol);
#else
    void* addr = dlsym(handle, symbol.c_str());
#endif

    if (addr == nullptr) {
        std::string message = "Failed to load symbol '" + std::string(symbol) + "' from library '" +
                              path.string() + "': ";

#ifdef _WIN32
        LPSTR buffer = nullptr;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr,
                       GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       reinterpret_cast<LPSTR>(&buffer), 0, nullptr);
        message += buffer;
        LocalFree(buffer);
#else
        message += dlerror();
#endif
        SetLastErrorMessage(message);

        return nullptr;
    }

    SetLastErrorMessage("");
    return addr;
}

} // namespace iocod
