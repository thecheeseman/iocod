macro(check_features)
    #
    # C++ Standards
    # require C++20 or later
    #
    list(FIND CMAKE_CXX_COMPILE_FEATURES "cxx_std_20" HAS_CXX20)
    if (NOT HAS_CXX20)
        message(SEND_ERROR "C++20 required")
    endif()

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
    set(CMAKE_CXX_EXTENSIONS TRUE)

    #
    # gather system information
    #
    cmake_host_system_information(RESULT _64bit QUERY IS_64BIT)
    cmake_host_system_information(RESULT _arch QUERY OS_PLATFORM)

    set(IOCOD_PLATFORM_64BIT ${_64bit} CACHE BOOL "64-bit build")
    set(IOCOD_PLATFORM_ARCH ${_arch} CACHE STRING "Build architecture")

    set(IOCOD_PLATFORM_OS ${CMAKE_SYSTEM_NAME} CACHE STRING "OS")
    string(TOLOWER ${IOCOD_PLATFORM_OS} IOCOD_PLATFORM_OS)
    set(IOCOD_PLATFORM_OS_VERSION ${CMAKE_SYSTEM_VERSION} CACHE STRING "OS version")
    if (NOT ${IOCOD_PLATFORM_OS_VERSION} STREQUAL "")
        string(TOLOWER ${IOCOD_PLATFORM_OS_VERSION} IOCOD_PLATFORM_OS_VERSION)
    endif()
    string(TOLOWER ${IOCOD_PLATFORM_ARCH} IOCOD_PLATFORM_ARCH)

    set(IOCOD_PLATFORM_WINDOWS FALSE CACHE BOOL "Windows build")
    set(IOCOD_PLATFORM_LINUX FALSE CACHE BOOL "Linux build")
    set(IOCOD_PLATFORM_MACOS FALSE CACHE BOOL "MacOS build")
    set(IOCOD_PLATFORM_EXE "" CACHE STRING "OS exectuable extension")
    set(IOCOD_PLATFORM_DLL "" CACHE STRING "OS shared library extension")

    if (${IOCOD_PLATFORM_OS} STREQUAL "windows")
        set(IOCOD_PLATFORM_WINDOWS TRUE)
        set(IOCOD_PLATFORM_EXE ".exe")
        set(IOCOD_PLATFORM_DLL ".dll")
    elseif (${IOCOD_PLATFORM_OS} STREQUAL "linux")
        set(IOCOD_PLATFORM_LINUX TRUE)
        set(IOCOD_PLATFORM_DLL ".so")
    elseif (${IOCOD_PLATFORM_OS} STREQUAL "darwin")
        set(IOCOD_PLATFORM_MACOS TRUE)
        set(IOCOD_PLATFORM_OS "macos")
        set(IOCOD_PLATFORM_DLL ".dylib")
    endif()
    message(DEBUG "Building on ${IOCOD_PLATFORM_OS}")

    #
    # arch
    #
    set(IOCOD_PLATFORM_AMD64 FALSE CACHE BOOL "amd64")
    set(IOCOD_PLATFORM_X86 FALSE CACHE BOOL "x86")
    set(IOCOD_PLATFORM_ARM32 FALSE CACHE BOOL "arm32")
    set(IOCOD_PLATFORM_ARM64 FALSE CACHE BOOL "arm64")

    if (${IOCOD_PLATFORM_ARCH} STREQUAL "x86_64" OR 
        ${IOCOD_PLATFORM_ARCH} STREQUAL "amd64")
        set(IOCOD_PLATFORM_ARCH "amd64")
        set(IOCOD_PLATFORM_AMD64 TRUE)
    elseif(${IOCOD_PLATFORM_ARCH} STREQUAL "aarch64")
        set(IOCOD_PLATFORM_ARCH "arm64")
        set(IOCOD_PLATFORM_ARM64 TRUE)
    elseif(${IOCOD_PLATFORM_ARCH} STREQUAL "x86")
        set(IOCOD_PLATFORM_X86 TRUE)
    elseif(${IOCOD_PLATFORM_ARCH} STREQUAL "aarch32")
        set(IOCOD_PLATFORM_ARCH "arm32")
        set(IOCOD_PLATFORM_ARM32 TRUE)
    endif()

    set(IOCOD_PLATFORM_BITS "64" CACHE STRING "'32' or '64''")
    if (IOCOD_PLATFORM_64BIT)
        option(BUILD64 "Build 64-bit binaries" ON)
        mark_as_advanced(BUILD64)

        if (NOT BUILD64)
            if (${IOCOD_PLATFORM_OS} STREQUAL "macos")
                message(SEND_ERROR "Cannot build 32-bit binaries on MacOS")
            endif()

            message(STATUS "Building 32-bit binaries on 64-bit system")
            set(IOCOD_PLATFORM_64BIT FALSE)
            set(IOCOD_PLATFORM_BITS "32")
            set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)

            # reset arch strings to be correct
            if (${IOCOD_PLATFORM_ARCH} STREQUAL "amd64")
                set(IOCOD_PLATFORM_ARCH "x86")
                set(IOCOD_PLATFORM_X86 TRUE)
            elseif (${IOCOD_PLATFORM_ARCH} STREQUAL "arm64")
                set(IOCOD_PLATFORM_ARCH "arm32")
                set(IOCOD_PLATFORM_ARM32 TRUE)
            endif()
        else()
            set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
        endif()
    endif()
    message(DEBUG "Building binaries for ${IOCOD_PLATFORM_ARCH}")

    #
    # compiler
    #
    set(IOCOD_PLATFORM_COMPILER ${CMAKE_CXX_COMPILER_ID} CACHE STRING "Compiler")
    string(TOLOWER ${IOCOD_PLATFORM_COMPILER} IOCOD_PLATFORM_COMPILER)
    set(IOCOD_PLATFORM_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION} CACHE STRING "Compiler version")

    set(IOCOD_PLATFORM_MSVC FALSE CACHE BOOL "MSVC build")
    set(IOCOD_PLATFORM_GCC FALSE CACHE BOOL "GCC build")
    set(IOCOD_PLATFORM_CLANG FALSE CACHE BOOL "Clang build")
    set(IOCOD_PLATFORM_MINGW FALSE CACHE BOOL "MinGW build")

    if (${IOCOD_PLATFORM_COMPILER} STREQUAL "appleclang" OR
        ${IOCOD_PLATFORM_COMPILER} STREQUAL "armclang")
        set(IOCOD_PLATFORM_CLANG TRUE)
    elseif (${IOCOD_PLATFORM_COMPILER} STREQUAL "gnu")
        set(IOCOD_PLATFORM_GCC TRUE)
        set(IOCOD_PLATFORM_COMPILER "gcc")
    elseif (${IOCOD_PLATFORM_COMPILER} STREQUAL "msvc")
        set(IOCOD_PLATFORM_MSVC TRUE)
    elseif (${IOCOD_PLATFORM_COMPILER} STREQUAL "mingw")
        set(IOCOD_PLATFORM_MINGW TRUE)
    endif()

    # compiler options for 32-bit
    set(IOCOD_PLATFORM_CFLAGS_32BIT "" CACHE STRING "Compiler flags for 32-bit")
    if (NOT IOCOD_PLATFORM_64BIT)
        if (IOCOD_PLATFORM_GCC OR IOCOD_PLATFORM_CLANG)
            set(IOCOD_PLATFORM_CFLAGS_32BIT "-m32")
        elseif (IOCOD_PLATFORM_MSVC)
            set(IOCOD_PLATFORM_CFLAGS_32BIT "-DWIN32")
        endif()
    endif()
    message(DEBUG "32-Bit compile flags: ${IOCOD_PLATFORM_CFLAGS_32BIT}")

    # debug flags

    #
    # endianness
    #
    set(IOCOD_PLATFORM_LITTLE_ENDIAN FALSE CACHE BOOL "Little-endian system")
    set(IOCOD_PLATFORM_BIG_ENDIAN FALSE CACHE BOOL "Big-endian system")

    include(CheckCXXSourceCompiles)
    set(CMAKE_REQUIRED_QUIET TRUE)
    check_cxx_source_compiles(
        "#include <inttypes.h>
        int main(void)
        {
            volatile uint32_t i = 0x01234567;
            
            // return 0 for big endian, 1 for little endian
            return (*((uint8_t *)(&i))) == 0x67;
        }" LITTLE_ENDIAN
    )

    if (LITTLE_ENDIAN)
        set(IOCOD_PLATFORM_LITTLE_ENDIAN TRUE)
    else()
        set(IOCOD_PLATFORM_BIG_ENDIAN TRUE)
    endif()

    #
    # compile flags
    #
    if (IOCOD_PLATFORM_MSVC)
        add_compile_options(/wd4996) # unsafe functions strcpy etc

        add_compile_definitions(UNICODE _UNICODE)

        string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    elseif (IOCOD_PLATFORM_GCC OR IOCOD_PLATFORM_CLANG OR IOCOD_PLATFORM_MINGW)
        add_compile_options(-Wall -Wextra)
    endif()
endmacro()
