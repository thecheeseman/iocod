// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SOURCE_LOCATION_H
#define CORE_SOURCE_LOCATION_H

#ifdef __has_builtin
    #if !__has_builtin(__builtin_FILE)
        #error "Compiler does not support __builtin_FILE"
    #elif !__has_builtin(__builtin_FUNCTION)
        #error "Compiler does not support __builtin_FUNCTION"
    #elif !__has_builtin(__builtin_LINE)
        #error "Compiler does not support __builtin_LINE"
    #endif
#endif

// STL replacement
class source_location {
public:
    constexpr source_location(const source_location&) = default;
    constexpr source_location(source_location&&) = default;
    constexpr source_location& operator=(const source_location&) = delete;
    constexpr source_location& operator=(source_location&&) = delete;
    constexpr ~source_location() = default;

    constexpr const char* file_name() const noexcept
    {
        return filename_;
    }

    constexpr const char* function_name() const noexcept
    {
        return function_;
    }

    constexpr int line() const noexcept
    {
        return line_;
    }

    static constexpr source_location current(const char* filename = __builtin_FILE(),
                                             const char* function = __builtin_FUNCTION(),
                                             int line = __builtin_LINE()) noexcept
    {
        return source_location{filename, function, line};
    })

private:
    constexpr source_location(const char* filename, const char* function, int line) noexcept
        : filename_(filename),
          function_(function),
          line_(line)
    {
    }

    const char* filename_{nullptr};
    const char* function_{nullptr};
    const int line_{0};
};

#endif // CORE_SOURCE_LOCATION_H
