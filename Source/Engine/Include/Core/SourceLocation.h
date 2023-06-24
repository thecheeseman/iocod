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
class SourceLocation {
public:
    constexpr SourceLocation(const SourceLocation&) = default;
    constexpr SourceLocation(SourceLocation&&) = default;
    constexpr SourceLocation& operator=(const SourceLocation&) = delete;
    constexpr SourceLocation& operator=(SourceLocation&&) = delete;
    constexpr ~SourceLocation() = default;

    constexpr const char* FileName() const noexcept
    {
        return m_filename;
    }

    constexpr const char* FunctionName() const noexcept
    {
        return m_function;
    }

    constexpr int Line() const noexcept
    {
        return m_line;
    }

    static constexpr SourceLocation Current(const char* filename = __builtin_FILE(),
                                            const char* function = __builtin_FUNCTION(),
                                            const int line = __builtin_LINE()) noexcept
    {
        return SourceLocation{filename, function, line};
    }

private:
    constexpr SourceLocation(const char* filename, const char* function, const int line) noexcept :
        m_filename(filename),
        m_function(function),
        m_line(line) {}

    const char* m_filename{nullptr};
    const char* m_function{nullptr};
    const int m_line{0};
};

#endif // CORE_SOURCE_LOCATION_H
