project(iccore)

file(GLOB IC_CORE_SRCS "*.c")

foreach (subdir
    cvar
    parse
    shared
    strings)
    file(GLOB sources "${subdir}/*.c")
    list(APPEND IC_CORE_SRCS ${sources})
endforeach()

include_directories(".")
include_directories(${PROJECT_BINARY_DIR})

add_library(iccore STATIC ${IC_CORE_SRCS})

add_subdirectory("tests")
