// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Platform/Platform.h>
#include <Platform/SharedLibrary.h>

namespace iocod {

// --------------------------------
// SharedLibrary::SharedLibrary
// --------------------------------
SharedLibrary::SharedLibrary(const String& libraryPath) noexcept
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
bool SharedLibrary::Load(const String& libraryPath) noexcept
{
    if (Loaded())
        Unload();

    m_path = libraryPath;
    m_loaded = false;

    m_handle = Platform::GetInstance().LibraryOpen(libraryPath);

    if (m_handle == nullptr) {
        const String message = "Failed to load library '" + libraryPath + "': " +
            Platform::GetInstance().GetLastErrorAsString();
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

    Platform::GetInstance().LibraryClose(m_handle);

    m_handle = nullptr;
    m_loaded = false;
}

// --------------------------------
// SharedLibrary::LoadVoidSymbol
// --------------------------------
void* SharedLibrary::LoadVoidSymbol(const char* symbol) noexcept
{
    if (!Loaded()) {
        SetLastErrorMessage("Tried to load symbol '" + String{symbol} +
            "' from an unloaded library");
        return nullptr;
    }

    const auto address = Platform::GetInstance().LibraryLoadSymbol(m_handle, symbol);

    if (address == nullptr) {
        const String message = "Failed to load symbol '" + String{symbol} + "' from library '" +
            m_path + "': " + Platform::GetInstance().GetLastErrorAsString();
        SetLastErrorMessage(message);

        return nullptr;
    }

    m_lastError.Clear();
    return address;
}

} // namespace iocod
