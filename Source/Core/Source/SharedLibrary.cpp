// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/SharedLibrary.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

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
bool SharedLibrary::Load(const char* library_path) noexcept
{
    if (Loaded())
        Unload();

    path = library_path;
    loaded = false;

#ifdef _WIN32
    // TODO: support wide strings
    handle = LoadLibraryA(library_path);
#else
    handle = dlopen(library_path, RTLD_LAZY);
#endif

    if (handle == nullptr) {
        std::string message = "Failed to load library '" + std::string(library_path) + "': ";

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

// ============================================================================
// C API
// ============================================================================

// --------------------------------
// shared_library_create
// --------------------------------
shared_library_t* shared_library_create(const char* path)
{
    if (path != nullptr)
        return reinterpret_cast<shared_library_t*>(new SharedLibrary(path));
    else
        return reinterpret_cast<shared_library_t*>(new SharedLibrary());
}

// --------------------------------
// shared_library_free
// --------------------------------
void shared_library_free(shared_library_t* library)
{
    if (library != nullptr)
        delete reinterpret_cast<SharedLibrary*>(library);
}

// --------------------------------
// shared_library_load
// --------------------------------
int shared_library_load(shared_library_t* library, const char* path)
{
    if (library == nullptr)
        return 0;

    return reinterpret_cast<SharedLibrary*>(library)->Load(path);
}

// --------------------------------
// shared_library_loaded
// --------------------------------
int shared_library_loaded(shared_library_t* library)
{
    if (library == nullptr)
        return 0;

    return reinterpret_cast<SharedLibrary*>(library)->Loaded();
}

// --------------------------------
// shared_library_unload
// --------------------------------
void shared_library_unload(shared_library_t* library)
{
    if (library == nullptr)
        return;

    reinterpret_cast<SharedLibrary*>(library)->Unload();
}

// --------------------------------
// shared_library_load_symbol
// --------------------------------
void* shared_library_load_symbol(shared_library_t* library, const char* symbol)
{
    if (library == nullptr)
        return nullptr;

    return reinterpret_cast<SharedLibrary*>(library)->LoadVoidSymbol(symbol);
}

// --------------------------------
// shared_library_error_message
// --------------------------------
const char* shared_library_error_message(shared_library_t* library)
{
    if (library == nullptr)
        return nullptr;

    static char error_message[1024]{0};
    strncpy(error_message, reinterpret_cast<SharedLibrary*>(library)->GetLastErrorMessage().c_str(),
            sizeof(error_message));
    return error_message;
}
// ============================================================================
