// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/Platform.h>
#include <Core/SharedLibrary.h>

#ifdef IOCOD_OS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

namespace iocod {

// --------------------------------
// SharedLibrary::SharedLibrary
// --------------------------------
SharedLibrary::SharedLibrary(const std::filesystem::path& libraryPath) noexcept
{
    Load(libraryPath);
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
    m_handle = other.m_handle;
    m_loaded = other.m_loaded;
    m_path = std::move(other.m_path);

    other.m_handle = nullptr;
    other.m_loaded = false;

    return *this;
}

// --------------------------------
// SharedLibrary::Loaded
// --------------------------------
bool SharedLibrary::Loaded() const noexcept
{
    return m_loaded;
}

// --------------------------------
// SharedLibrary::Load
// --------------------------------
bool SharedLibrary::Load(const std::filesystem::path& libraryPath) noexcept
{
    if (Loaded())
        Unload();

    m_path = libraryPath;
    m_loaded = false;

#ifdef IOCOD_OS_WINDOWS
    // TODO: support wide strings
    m_handle = LoadLibraryA(libraryPath.string().c_str());
#else
    m_handle = dlopen(libraryPath.c_str(), RTLD_LAZY);
#endif

    if (m_handle == nullptr) {
        String message = "Failed to load library '" + libraryPath.string() + "': ";

#ifdef IOCOD_OS_WINDOWS
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
        m_loaded = true;
    }

    return m_loaded;
}

// --------------------------------
// SharedLibrary::Unload
// --------------------------------
void SharedLibrary::Unload() noexcept
{
    if (!Loaded())
        return;

#ifdef IOCOD_OS_WINDOWS
    FreeLibrary(static_cast<HMODULE>(m_handle));
#else
    dlclose(m_handle);
#endif

    m_handle = nullptr;
    m_loaded = false;
}

// --------------------------------
// SharedLibrary::LoadVoidSymbol
// --------------------------------
void* SharedLibrary::LoadVoidSymbol(const char* symbol) noexcept
{
    if (!Loaded()) {
        SetLastErrorMessage("Tried to load symbol '" + String(symbol) +
                            "' from an unloaded library");
        return nullptr;
    }

#ifdef IOCOD_OS_WINDOWS
    auto address = static_cast<void*>(GetProcAddress(static_cast<HMODULE>(m_handle), symbol));
#else
    auto address = dlsym(m_handle, symbol.c_str());
#endif

    if (address == nullptr) {
        String message = "Failed to load symbol '" + String(symbol) + "' from library '" +
                              m_path.string() + "': ";

#ifdef IOCOD_OS_WINDOWS
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

    m_lastError.clear();
    return address;
}

} // namespace iocod
