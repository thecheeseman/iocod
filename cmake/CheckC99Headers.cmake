# Check for C99 headers

macro(CHECK_C99_HEADERS)
    if (NOT CMAKE_REQUIRED_QUIET)
        message(STATUS "Checking for C99 header files")
    endif()
    
    check_include_files("complex.h;fenv.h;inttypes.h;stdbool.h;stdint.h;tgmath.h" STDC99_HEADERS)
    if (STDC_HEADERS)
        message(STATUS "C99 headers found")

        set(HAVE_COMPLEX_H 1)
        set(HAVE_FENV_H 1)
        set(HAVE_INTTYPES_H 1)
        set(HAVE_STDBOOL_H 1)
        set(HAVE_STDINT_H 1)
        set(HAVE_TGMATH_H 1)
    else()
        message(STATUS "Couldn't find all C99 headers, checking individually")

    check_include_files(complex.h HAVE_COMPLEX_H)
    check_include_files(fenv.h HAVE_FENV_H)
    check_include_files(inttypes.h HAVE_INTTYPES_H)
    check_include_files(stdbool.h HAVE_STDBOOL_H)
    check_include_files(stdint.h HAVE_STDINT_H)
    check_include_files(tgmath.h HAVE_TGMATH_H)
    endif()
endmacro()
