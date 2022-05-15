project(iocodmain)

#
# headers
#
#include_directories("${CMAKE_SOURCE_DIR}/include")          # API headers
include_directories("external")     # TODO: this needs to be moved to /external/
include_directories("include")      # internal headers
include_directories(".")

#
# link directory
#
link_directories("${CMAKE_SOURCE_DIR}/lib")

# 1. iocod -> main server exe
if (WIN32)
    add_executable(iocod WIN32 "")
else()
    add_executable(iocod "")
endif()

# 2. iocod_game -> game code
add_library(iocod_game SHARED "")

#
# iocod sources
#
file(GLOB IOCOD_SOURCES "*.c")

foreach (subdir
    clipmap
    commands
    common 
    compat
    cvar 
    external 
    files 
    network 
    script 
    server
    server/syscalls
    shared
    sound 
    strings
    system
    system/null)
    file(GLOB sources "${subdir}/*.c")
    list(APPEND IOCOD_SOURCES ${sources})
endforeach()

if (WIN32)
    file(GLOB sys "system/windows/*.c")
    list(APPEND IOCOD_SOURCES ${sys})
else()
    file(GLOB sys "system/unix/*.c")
    list(APPEND IOCOD_SOURCES ${sys})
endif()

target_sources(iocod PRIVATE ${IOCOD_SOURCES})

#
# game dll sources
#
file(GLOB GAME_SOURCES "*.c")

# remove any common files that the game dll doesn't need
foreach (f 
        vm)
    get_filename_component(p "${CMAKE_CURRENT_SOURCE_DIR}/${f}.c" ABSOLUTE)
    list(REMOVE_ITEM GAME_SOURCES ${p})
endforeach()

foreach (subdir
    compat
    game
    game/bg
    game/script
    shared
    strings)
    file(GLOB sources "${subdir}/*.c")
    list(APPEND GAME_SOURCES ${sources})
endforeach()

if (USE_LUA)
    file(GLOB lua "game/lua/*.c")
    list(APPEND GAME_SOURCES ${lua})
endif()

# other common stuff
list(APPEND GAME_SOURCES "common/memory.c")

target_sources(iocod_game PRIVATE ${GAME_SOURCES})

# 
target_compile_options(iocod PRIVATE "${IC_PLATFORM_BIT_OPS}")
target_link_options(iocod PRIVATE "${IC_PLATFORM_BIT_OPS}")
target_compile_options(iocod_game PRIVATE "${IC_PLATFORM_BIT_OPS}")
target_link_options(iocod_game PRIVATE "${IC_PLATFORM_BIT_OPS}")

set_target_properties(iocod PROPERTIES OUTPUT_NAME "iocod${PLAT_BITS}")

#
# PREFIX disable "lib" prefix on Unix systems
# LIBRARY_OUTPUT_DIRECTORY overrides the settings we set at the top
# so that the static libraries get put in lib/ and this dll gets put
# with the binaries
#
set_target_properties(iocod_game PROPERTIES
    PREFIX ""
    OUTPUT_NAME "icgame${IC_PLATFORM_BITS}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/main"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/main")

#
# linker options
#
if (WIN32)
    target_link_libraries(iocod dl getopt winmm miniz curl)
    target_link_libraries(iocod_game)
else()
    target_link_libraries(iocod ${CMAKE_DL_LIBS} m miniz curl)
    target_link_libraries(iocod_game m)
endif()
