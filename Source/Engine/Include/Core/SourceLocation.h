// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SOURCE_LOCATION_H
#define CORE_SOURCE_LOCATION_H

#ifdef __has_builtin
#if !__has_builtin(__builtin_LINE)
        #error "Compiler does not support __builtin_LINE"
#elif !__has_builtin(__builtin_FILE)
        #error "Compiler does not support __builtin_FILE"
#elif !__has_builtin(__builtin_FUNCTION)
        #error "Compiler does not support __builtin_FUNCTION"
#elif !__has_builtin(__builtin_COLUMN)
        #error "Compiler does not support __builtin_COLUMN"
#endif
#endif

// STL replacement
class SourceLocation {
public:
    static constexpr SourceLocation Current(const std::uint_least32_t line = __builtin_LINE(),
                                            const std::uint_least32_t column = __builtin_COLUMN(),
                                            const char* filename = __builtin_FILE(),
                                            const char* function = __builtin_FUNCTION()) noexcept
    {
        return SourceLocation(line, column, filename, function);
    }

    [[nodiscard]] constexpr std::uint_least32_t Line() const noexcept { return m_line; }
    [[nodiscard]] constexpr std::uint_least32_t Column() const noexcept { return m_column; }
    [[nodiscard]] constexpr const char* FileName() const noexcept { return m_filename; }
    [[nodiscard]] constexpr const char* FunctionName() const noexcept { return m_functionName; }

private:
    constexpr SourceLocation() :
        m_line(0),
        m_column(0),
        m_filename(nullptr),
        m_functionName(nullptr) {}

    constexpr SourceLocation(const std::uint_least32_t line,
                             const std::uint_least32_t column,
                             const char* filename,
                             const char* function) :
        m_line(line),
        m_column(column),
        m_filename(filename),
        m_functionName(function) {}

    std::uint_least32_t m_line;
    std::uint_least32_t m_column;
    const char* m_filename;
    const char* m_functionName;
};

#endif // CORE_SOURCE_LOCATION_H
