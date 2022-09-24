#include "iocod.h"
#include <string>
#include <iostream>

#ifdef IOCOD_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace iocod::system {

class shared_library::impl {
    #ifdef IOCOD_PLATFORM_WINDOWS
    using handle_type = HMODULE;
    #else
    using handle_type = void*;
    #endif

    static inline i32 g_unique_id { 0 };
    i32 unique_id { 0 };
    handle_type handle {nullptr};
    bool loaded {false};
    std::filesystem::path path {};
public:

    impl() = default;
    ~impl() = default;

    bool is_loaded() const
    {
        return loaded;
    }

    bool load(const std::filesystem::path& path_)
    {
        if (is_loaded()) {
            std::cout << "[shared_library:" << unique_id << 
                "] Unloading previously loaded library: " << path << "\n";
            unload();
        } else {
            unique_id = g_unique_id++;
        }
        
        path = path_;
        loaded = false;

        std::cout << "[shared_library:" << unique_id << "] Loading library " << path << "\n";

        #ifdef IOCOD_PLATFORM_WINDOWS
        handle = LoadLibrary(path.c_str());
        #else
        handle = dlopen(path.c_str(), RTLD_LAZY);
        #endif

        if (handle == nullptr) {
            std::error_code ec = system::get_last_error();
            std::cout << "[shared_library:" << unique_id << "] Failed to load library " << path << ": " << ec.message() << "\n";
        } else {
            loaded = true;
        }

        return loaded;
    }

    void unload()
    {
        if (!is_loaded()) {
            std::cout << "[shared_library] Can't unload unloaded library\n";
            return;
        }

        std::cout << "[shared_library:" << unique_id << "] Unloading library " << path << "\n";

        #ifdef IOCOD_PLATFORM_WINDOWS
        FreeLibrary(handle);
        #else
        dlclose(handle);
        #endif

        handle = nullptr;
        loaded = false;
    }

    void* get_void(const char* symbol) const
    {
        if (!is_loaded()) {
            std::cout << "[shared_library:" << unique_id << "] Can't get symbol '" << symbol
                << "' from unloaded library\n";
            return nullptr;
        }

        void* addr = nullptr;
        #ifdef IOCOD_PLATFORM_WINDOWS
        addr = GetProcAddress(handle, symbol);
        #else
        addr = dlsym(handle, symbol);
        #endif

        if (addr == nullptr) {
            std::error_code ec = system::get_last_error();
            std::cout << "[shared_library:" << unique_id << "] Failed to get symbol '"
                << symbol << "' from library " <<
                path << ": " << ec.message() << "\n";
        }

        return addr;
    }
};

shared_library::shared_library() : pimpl(new impl)
{

}

shared_library::shared_library(const std::filesystem::path& path_) : pimpl(new impl)
{
    load(path_);
}

shared_library::~shared_library()
{
    if (is_loaded())
        unload();
}

void* shared_library::get_void(const char* symbol) const
{
    return pimpl->get_void(symbol);
}

bool shared_library::load(const std::filesystem::path& path_)
{
    return pimpl->load(path_);
}

bool shared_library::is_loaded() const
{
    return pimpl->is_loaded();
}

void shared_library::unload()
{
    return pimpl->unload();
}

// namespace iocod::system
};
