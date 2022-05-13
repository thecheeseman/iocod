cmake_host_system_information(RESULT IC_64BIT       QUERY IS_64BIT)
cmake_host_system_information(RESULT IC_PLATFORM    QUERY OS_PLATFORM)
set(IC_OS ${CMAKE_HOST_SYSTEM_NAME})

string(TOLOWER ${IC_OS} IC_OS)
string(TOLOWER ${IC_PLATFORM} IC_PLATFORM)

# normalise architectures
if (${IC_PLATFORM} STREQUAL "x86_64")
    set(IC_PLATFORM "amd64")
endif()

# Linux usually reports which arm version (e.g. armv7l)
# Windows/Mac report arm64
string(FIND ${IC_PLATFORM} "arm" IS_ARM)
if (${IS_ARM} EQUAL 0)
    if (IC_64BIT)
        set(IC_PLATFORM "arm64")
    else()
        set(IC_PLATFORM "arm32")
    endif()
endif()
