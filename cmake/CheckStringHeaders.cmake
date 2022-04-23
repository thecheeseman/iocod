#
macro(check_string_headers)
    if (NOT CMAKE_REQUIRED_QUIET)
        message(STATUS "Checking for string header symbols")
    endif()

    check_symbol_exists(strdup "string.h" HAVE_STRDUP)
    check_symbol_exists(strndup "string.h" HAVE_STRNDUP)
    check_symbol_exists(strlwr "string.h" HAVE_STRLWR)
    check_symbol_exists(strupr "string.h" HAVE_STRUPR)

    check_include_files(strings.h HAVE_STRINGS_H)
    if (HAVE_STRINGS_H)
        check_symbol_exists(strcasecmp "strings.h" HAVE_STRCASECMP)
        check_symbol_exists(strncasecmp "strings.h" HAVE_STRNCASECMP)
    endif()
endmacro()
