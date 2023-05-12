// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SHARED_LIBRARY_H
#define CORE_SHARED_LIBRARY_H

#ifdef __cplusplus
    #include <filesystem>
    #include <string>

class SharedLibrary final {
public:
    SharedLibrary() noexcept = default;
    ~SharedLibrary();

    SharedLibrary(SharedLibrary&& other) noexcept;
    SharedLibrary& operator=(SharedLibrary&& other) noexcept;
    SharedLibrary(const SharedLibrary&) = delete;
    SharedLibrary& operator=(const SharedLibrary&) = delete;

    /// @brief Construct a new Shared Library object. Automatically loads the library.
    /// @param[in] library_path path to the shared library
    explicit SharedLibrary(const std::filesystem::path& library_path) noexcept;

    /// @brief Load the library.
    /// @param[in] library_path path to the shared library
    /// @return true if the library was loaded successfully
    bool Load(const char* library_path) noexcept;

    bool Load(const std::filesystem::path& library_path) noexcept
    {
        return Load(library_path.string().c_str());
    }

    /// @brief Unload the library.
    void Unload() noexcept;

    /// @brief Check if the library is loaded.
    /// @return true if the library is loaded
    bool Loaded() const noexcept;

    /// @brief Get the last error, if any.
    /// @return const std::string last error
    inline const std::string GetLastErrorMessage() const noexcept
    {
        return last_error;
    }

    // @brief Load a symbol from the library.
    /// @tparam T type of the symbol
    /// @param[in] name name of the symbol
    /// @return T symbol or nullptr if failed
    template <typename T>
    inline T LoadSymbol(const std::string& name) noexcept
    {
        return reinterpret_cast<T>(LoadVoidSymbol(name));
    }

    template <typename T>
    inline T LoadSymbol(const char* symbol) noexcept
    {
        return reinterpret_cast<T>(LoadVoidSymbol(symbol));
    }

    void* LoadVoidSymbol(const char* symbol) noexcept;

private:
    void* handle = nullptr;
    bool loaded = false;
    std::filesystem::path path{};
    std::string last_error{};

    inline void SetLastErrorMessage(const std::string& error) noexcept
    {
        last_error = error;
    }
};
#endif

// ============================================================================
// C API
// ============================================================================
#ifdef __cplusplus
extern "C" {
#endif

typedef struct shared_library_t shared_library_t;

/// @brief Create a new shared library object.
/// @param[in] path path to the shared library, or NULL to create an empty object
/// @return shared library object or NULL if failed
shared_library_t* shared_library_create(const char* path);

/// @brief Free a shared library object.
/// @param[in] library shared library object
void shared_library_free(shared_library_t* library);

/// @brief Load a shared library.
/// @param[in] library shared library object
/// @param[in] path path to the shared library
/// @return 1 if the library was loaded successfully, 0 otherwise
int shared_library_load(shared_library_t* library, const char* path);

/// @brief Check if a shared library is loaded.
/// @param[in] library shared library object
/// @return 1 if the library is loaded, 0 otherwise
int shared_library_loaded(shared_library_t* library);

/// @brief Unload a shared library.
/// @param[in] library shared library object
void shared_library_unload(shared_library_t* library);

/// @brief Load a symbol from a shared library.
/// @param[in] library shared library object
/// @param[in] symbol name of the symbol
/// @return pointer to the symbol or NULL if failed
void* shared_library_load_symbol(shared_library_t* library, const char* symbol);

/// @brief Get the last error message.
/// @param[in] library shared library object
/// @return last error message
const char* shared_library_error_message(shared_library_t* library);

#ifdef __cplusplus
}
#endif

#endif // CORE_SHARED_LIBRARY_H
