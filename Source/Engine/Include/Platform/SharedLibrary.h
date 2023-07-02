// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_PLATFORM_SHARED_LIBRARY_H
#define ENGINE_PLATFORM_SHARED_LIBRARY_H

#include <Core/String.h>

namespace iocod {

/**
 * \brief Wrapper for handling shared libraries. 
 */
class IOCOD_API SharedLibrary final {
public:
    SharedLibrary() noexcept = default;
    ~SharedLibrary();

    SharedLibrary(SharedLibrary&& other) noexcept;
    SharedLibrary& operator=(SharedLibrary&& other) noexcept;
    SharedLibrary(const SharedLibrary&) = delete;
    SharedLibrary& operator=(const SharedLibrary&) = delete;

    /**
     * \brief Construct a new Shared Library object, automatically loading the library.
     * \param[in] libraryPath path to the shared library
     */
    explicit SharedLibrary(const String& libraryPath) noexcept;

    /**
     * \brief Load the library at the given path.
     * \param[in] libraryPath path to the shared library
     * \return true if the library was loaded successfully
     */
    bool Load(const String& libraryPath) noexcept;

    /**
     * \brief Unload the library.
     */
    void Unload() noexcept;

    /**
     * \brief Check if the library is loaded.
     * \return true if the library is loaded
     */
    [[nodiscard]] bool Loaded() const noexcept;

    /**
     * \brief Get the last error, if any.
     * \return String last error
     */
    [[nodiscard]] const String& GetLastErrorMessage() const noexcept
    {
        return m_lastError;
    }

    /**
     * \brief Load a symbol from the library.
     * \tparam T symbol type
     * \param[in] name symbol name
     * \return T symbol or nullptr if the symbol was not found
     */
    template <typename T>
    T LoadSymbol(const String& name) noexcept
    {
        return reinterpret_cast<T>(LoadVoidSymbol(name.c_str()));
    }

    void* LoadVoidSymbol(const char* symbol) noexcept;

private:
    void* m_handle = nullptr;
    bool m_loaded = false;
    String m_path{};
    String m_lastError{};

    void SetLastErrorMessage(const String& error) noexcept
    {
        m_lastError = error;
    }
};

} // namespace iocod

#endif // ENGINE_PLATFORM_SHARED_LIBRARY_H
