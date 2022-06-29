macro(check_features)
    # gather system information
    cmake_host_system_information(RESULT _64bit QUERY IS_64BIT)
    cmake_host_system_information(RESULT _arch QUERY OS_PLATFORM)

    set(IC_PLATFORM_64BIT ${_64bit} CACHE BOOL "64-bit build")
    set(IC_PLATFORM_ARCH ${_arch} CACHE STRING "Build architecture")

    set(IC_PLATFORM_OS ${CMAKE_SYSTEM_NAME} CACHE STRING "OS")
    string(TOLOWER ${IC_PLATFORM_OS} IC_PLATFORM_OS)
    set(IC_PLATFORM_OS_VERSION ${CMAKE_SYSTEM_VERSION} CACHE STRING "OS version")
    string(TOLOWER ${IC_PLATFORM_OS_VERSION} IC_PLATFORM_OS_VERSION)
    string(TOLOWER ${IC_PLATFORM_ARCH} IC_PLATFORM_ARCH)

    set(IC_PLATFORM_WINDOWS FALSE CACHE BOOL "Windows build")
    set(IC_PLATFORM_LINUX FALSE CACHE BOOL "Linux build")
    set(IC_PLATFORM_MACOS FALSE CACHE BOOL "MacOS build")
    set(IC_PLATFORM_EXE "" CACHE STRING "OS exectuable extension")
    set(IC_PLATFORM_DLL "" CACHE STRING "OS shared library extension")

    if (${IC_PLATFORM_OS} STREQUAL "windows")
        set(IC_PLATFORM_WINDOWS TRUE)
        set(IC_PLATFORM_EXE ".exe")
        set(IC_PLATFORM_DLL ".dll")
    elseif (${IC_PLATFORM_OS} STREQUAL "linux")
        set(IC_PLATFORM_LINUX TRUE)
        set(IC_PLATFORM_DLL ".so")
    elseif (${IC_PLATFORM_OS} STREQUAL "darwin")
        set(IC_PLATFORM_MACOS TRUE)
        set(IC_PLATFORM_OS "macos")
        set(IC_PLATFORM_DLL ".dylib")
    endif()
    message(DEBUG "Building on ${IC_PLATFORM_OS}")

    # arch
    set(IC_PLATFORM_AMD64 FALSE CACHE BOOL "AMD64")
    set(IC_PLATFORM_X86 FALSE CACHE BOOL "x86")
    set(IC_PLATFORM_ARM32 FALSE CACHE BOOL "arm32")
    set(IC_PLATFORM_ARM64 FALSE CACHE BOOL "arm64")

    if (${IC_PLATFORM_ARCH} STREQUAL "x86_64")
        set(IC_PLATFORM_ARCH "amd64")
        set(IC_PLATFORM_AMD64 TRUE)
    elseif(${IC_PLATFORM_ARCH} STREQUAL "aarch64")
        set(IC_PLATFORM_ARM64 TRUE)
    endif()

    set(IC_PLATFORM_BITS "64" CACHE STRING "'32' or '64''")
    if (IC_PLATFORM_64BIT)
        option(BUILD64 "Build 64-bit binaries" ON)
        mark_as_advanced(BUILD64)

        if (NOT BUILD64)
            if (${IC_PLATFORM_OS} STREQUAL "macos")
                message(SEND_ERROR "Cannot build 32-bit binaries on MacOS")
            endif()

            message(STATUS "Building 32-bit binaries on 64-bit system")
            set(IC_PLATFORM_64BIT FALSE)
            set(IC_PLATFORM_BITS "32")
            set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)

            # reset arch strings to be correct
            if (${IC_PLATFORM_ARCH} STREQUAL "amd64")
                set(IC_PLATFORM_ARCH "x86")
                set(IC_PLATFORM_X86 TRUE)
            elseif (${IC_PLATFORM_ARCH} STREQUAL "aarch64")
                set(IC_PLATFORM_ARCH "aarch32")
                set(IC_PLATFORM_ARM32 TRUE)
            endif()
        else()
            set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
        endif()
    endif()
    message(DEBUG "Building binaries for ${IC_PLATFORM_ARCH}")

    # compiler
    set(IC_PLATFORM_COMPILER ${CMAKE_C_COMPILER_ID} CACHE STRING "Compiler")
    string(TOLOWER ${IC_PLATFORM_COMPILER} IC_PLATFORM_COMPILER)
    set(IC_PLATFORM_COMPILER_VERSION ${CMAKE_C_COMPILER_VERSION} CACHE STRING "Compiler version")

    set(IC_PLATFORM_MSVC FALSE CACHE BOOL "MSVC build")
    set(IC_PLATFORM_GCC FALSE CACHE BOOL "GCC build")
    set(IC_PLATFORM_CLANG FALSE CACHE BOOL "Clang build")
    set(IC_PLATFORM_MINGW FALSE CACHE BOOL "MinGW build")

    if (${IC_PLATFORM_COMPILER} STREQUAL "appleclang" OR
        ${IC_PLATFORM_COMPILER} STREQUAL "armclang")
        set(IC_PLATFORM_CLANG TRUE)
    elseif (${IC_PLATFORM_COMPILER} STREQUAL "gnu")
        set(IC_PLATFORM_GCC TRUE)
        set(IC_PLATFORM_COMPILER "gcc")
    elseif (${IC_PLATFORM_COMPILER} STREQUAL "msvc")
        set(IC_PLATFORM_MSVC TRUE)
    elseif (${IC_PLATFORM_COMPILER} STREQUAL "mingw")
        set(IC_PLATFORM_MINGW TRUE)
    endif()

    # compiler options for 32-bit
    set(IC_PLATFORM_CFLAGS_32BIT "" CACHE STRING "Compiler flags for 32-bit")
    if (NOT IC_PLATFORM_64BIT)
        if (IC_PLATFORM_GCC OR IC_PLATFORM_CLANG)
            set(IC_PLATFORM_CFLAGS_32BIT "-m32")
        elseif (IC_PLATFORM_MSVC)
            set(IC_PLATFORM_CFLAGS_32BIT "-DWIN32")
        endif()
    endif()
    message(DEBUG "32-Bit compile flags: ${IC_PLATFORM_CFLAGS_32BIT}")

    # debug flags
endmacro()
