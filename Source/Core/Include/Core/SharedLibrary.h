// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SHARED_LIBRARY_H
#define CORE_SHARED_LIBRARY_H

#include <filesystem>
#include <Core/Types.h>

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
    /// @param[in] libraryPath m_path to the shared library
    explicit SharedLibrary(const std::filesystem::path& libraryPath) noexcept;

    /// @brief Load the library.
    /// @param[in] libraryPath m_path to the shared library
    /// @return true if the library was m_loaded successfully
    bool Load(const std::filesystem::path& libraryPath) noexcept;

    /// @brief Unload the library.
    void Unload() noexcept;

    /// @brief Check if the library is m_loaded.
    /// @return true if the library is m_loaded
    [[nodiscard]] bool Loaded() const noexcept;

    /// @brief Get the last error, if any.
    /// @return const std::string last error
    [[nodiscard]] const String& GetLastErrorMessage() const noexcept
    {
        return m_lastError;
    }

    // @brief Load a symbol from the library.
    /// @tparam T type of the symbol
    /// @param[in] name name of the symbol
    /// @return T symbol or nullptr if failed
    template <typename T>
    T LoadSymbol(const String& name) noexcept
    {
        return reinterpret_cast<T>(LoadVoidSymbol(name.c_str()));
    }

    void* LoadVoidSymbol(const char* symbol) noexcept;

private:
    void* m_handle = nullptr;
    bool m_loaded = false;
    std::filesystem::path m_path{};
    String m_lastError{};

    void SetLastErrorMessage(const String& error) noexcept
    {
        m_lastError = error;
    }
};

} // namespace iocod

#endif // CORE_SHARED_LIBRARY_H
