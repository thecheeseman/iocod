# APPLE
# CMAKE_SIZEOF_VOID_P
# CYGWIN
# IOS
# MINGW
# MSVC
# MSYS
# UNIX
# WIN32
# CMAKE_SYSTEM_NAME
# CMAKE_SYSTEM_PROCESSOR
# CMAKE_SYSTEM_VERSION
# CMAKE_C_BYTE_ORDER (3.20)
# CMAKE_C_COMPILER_ID
# CMAKE_C_COMPILER_VERSION
cmake_host_system_information(RESULT iocod_64BIT    QUERY IS_64BIT)
cmake_host_system_information(RESULT iocod_PLATFORM QUERY OS_PLATFORM)

set(iocod_OS ${CMAKE_HOST_SYSTEM_NAME})
string(TOLOWER ${iocod_OS} iocod_OS)
string(TOLOWER ${iocod_PLATFORM} iocod_PLATFORM)

#
# compilers
#
set(iocod_COMPILER "${CMAKE_C_COMPILER_ID}")
string(TOLOWER ${iocod_COMPILER} iocod_COMPILER)

# normalise compiler strings
if (${iocod_COMPILER} STREQUAL "appleclang")
    set(iocod_COMPILER "clang")
endif()

if (MSVC)
    set(IC_CMAKE_PLATFORM_MSVC TRUE)
elseif (MINGW)
    set(iocod_COMPILER "mingw")
    set(IC_CMAKE_PLATFORM_MINGW TRUE)
elseif (${iocod_COMPILER} STREQUAL "gnu")
    set(iocod_COMPILER "gcc")
    set(IC_CMAKE_PLATFORM_GCC TRUE)
elseif (${iocod_COMPILER} STREQUAL "clang")
    set(IC_CMAKE_PLATFORM_CLANG TRUE)
endif()

set(iocod_COMPILER_VER "${CMAKE_C_COMPILER_VERSION}")

#
# operating systems
#
if (WIN32)
    set(IC_CMAKE_PLATFORM_WINDOWS TRUE)
elseif (APPLE)
    set(IC_CMAKE_PLATFORM_MACOS TRUE)
elseif (${iocod_OS} STREQUAL "linux")
    set(IC_CMAKE_PLATFORM_LINUX TRUE)
endif()

# normalise operating system strings
if (${iocod_OS} STREQUAL "windows")
    set(iocod_OS "win")
elseif (${iocod_OS} STREQUAL "darwin")
    set(iocod_OS "macos")
endif()

#
# arch
#
set(iocod_ARCH "${iocod_PLATFORM}")

# Linux usually reports which arm version (e.g. armv7l)
# Windows/Mac report arm64
string(FIND ${iocod_PLATFORM} "arm" IS_ARM)
if (${IS_ARM} EQUAL 0)
    if (iocod_64BIT)
        set(iocod_ARCH "arm64")
    else()
        set(iocod_ARCH "arm32")
    endif()
endif()

# if we're building on a 64-bit system, allow the user to optionally
# force build a 32-bit version
if (iocod_64BIT)
    option(BUILD_32_BIT "Build 32-bit binaries" FALSE)

    if (BUILD_32_BIT)
        if (${iocod_ARCH} STREQUAL "x86_64" OR ${iocod_ARCH} STREQUAL "amd64")
            set(iocod_ARCH "x86")
        endif()

        if (${iocod_ARCH} STREQUAL "arm64")
            set(iocod_ARCH "arm32")
        endif()
    else()
        set(IC_CMAKE_PLATFORM_64BIT TRUE)
    endif()
endif()
