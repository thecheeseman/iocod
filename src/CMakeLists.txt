#
# iocod sources
#
file(GLOB IOCOD_SOURCES "*.c")

foreach (subdir
    client
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
    system)
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
    shared)
    file(GLOB sources "${subdir}/*.c")
    list(APPEND GAME_SOURCES ${sources})
endforeach()

if (USE_LUA)
    file(GLOB lua "game/lua/*.c")
    list(APPEND GAME_SOURCES ${lua})
endif()

# other common stuff
list(APPEND GAME_SOURCES "common/memory.c")

target_sources(game PRIVATE ${GAME_SOURCES})
