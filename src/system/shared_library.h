#pragma once
#ifndef SYSTEM_LIBRARY_H
#define SYSTEM_LIBRARY_H

#include <filesystem>

namespace iocod::system {

class shared_library {
    class impl;
    std::unique_ptr<impl> pimpl;

public:
    /**
     * @brief Default constructor, no shared library is loaded.
    */
    shared_library();

    /**
     * @brief Load the shared library at the given path.
     * @param[in] path_ path to the shared library
    */
    shared_library(const std::filesystem::path& path_);

    ~shared_library();

    /**
     * @brief Load the shared library at the given path. Automatically unloads
     * a previously loaded library, if any.
     * @param[in] path_ path to the shared library
     * @return true if the library was loaded successfully, false otherwise
    */
    bool load(const std::filesystem::path& path_);

    /**
     * @brief Unload the shared library.
    */
    void unload();

    /**
     * @brief Check if a shared library is loaded.
     * @return true if a shared library is loaded, false otherwise
    */
    bool is_loaded() const;

    /**
     * @brief Get a pointer to a function from the library.
     * @param[in] name the name of the function
     * @return a pointer to the function, or nullptr if the function was not found
     */
    template <typename T>
    inline T get(const char* name) const
    {
        return reinterpret_cast<T>(get_void(name));
    }

private:
    void* get_void(const char* symbol) const;
};

// namespace iocod::system
};

#endif /* SYSTEM_LIBRARY_H */
