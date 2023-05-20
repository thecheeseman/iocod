// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SHARED_LIBRARY_H
#define CORE_SHARED_LIBRARY_H

#include <filesystem>
#include <string>

namespace iocod {

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
    bool Load(const std::filesystem::path& library_path) noexcept;

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
        return reinterpret_cast<T>(LoadVoidSymbol(name.c_str()));
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

} // namespace iocod

#endif // CORE_SHARED_LIBRARY_H
