cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(core_tests C)

include(CTest)
add_custom_target(check 
    COMMAND ${CMAKE_CTEST_COMMAND} 
        --output-on-failure -C ${CMAKE_CFG_INTDIR}
)

function(add_core_test)
    set(one_value_args NAME)
    set(multi_value_args SOURCES WIN32_LIBS MACOS_LIBS LINUX_LIBS)
    cmake_parse_arguments(PARSE_ARGV 0 CT
        "" 
        "${one_value_args}" 
        "${multi_value_args}" 
    )
    add_executable(${CT_NAME} ${CT_SOURCES})

    if (NOT IC_PLATFORM_64BIT)
        target_compile_options(${CT_NAME} BEFORE PUBLIC ${IC_PLATFORM_CFLAGS_32BIT})
    endif()

    add_test(NAME ${CT_NAME} 
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${CT_NAME}>
        COMMAND $<TARGET_FILE:${CT_NAME}>)
    add_dependencies(check ${CT_NAME})
    target_link_libraries(${CT_NAME} iocod_core)

    if (IC_PLATFORM_WINDOWS AND DEFINED CT_WIN32_LIBS)
        target_link_libraries(${CT_NAME} ${CT_WIN32_LIBS})
    endif()

    if (IC_PLATFORM_LINUX AND DEFINED CT_LINUX_LIBS)
        target_link_libraries(${CT_NAME} ${CT_LINUX_LIBS})
    endif()

    if (IC_PLATFORM_MACOS AND DEFINED CT_MACOS_LIBS)
        target_link_libraries(${CT_NAME} ${CT_MACOS_LIBS})
    endif()

    if (IC_PLATFORM_WINDOWS)
        target_link_options(${CT_NAME} BEFORE PUBLIC "/SUBSYSTEM:CONSOLE,5.01")
    endif()

    set_target_properties(${CT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/tests/${CT_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/tests/${CT_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/tests/${CT_NAME}")

    message(STATUS "Added iccore test '${CT_NAME}' (${CT_SOURCES})")
endfunction()

# special tests that require more arguments
list(APPEND SPECIAL_TESTS 
    "test-network" 
    "test-netadr" 
    "test-console" 
    "test-library"
)

file(GLOB TEST_SRCS "*.c")
foreach (_test IN LISTS TEST_SRCS)
    # strip file path to get clean test name
    get_filename_component(_name ${_test} NAME)
    string(REPLACE ".c" "" _name ${_name})

    set(_skip FALSE)
    foreach (_spt IN LISTS SPECIAL_TESTS)
        if (${_spt} STREQUAL ${_name})
            set(_skip TRUE)
            break()
        endif()
    endforeach()

    if (NOT _skip)
        add_core_test(NAME ${_name} SOURCES ${_test})
    endif()
endforeach()

add_core_test(NAME "test-network"   SOURCES "test-network.c"    WIN32_LIBS "Ws2_32")
add_core_test(NAME "test-netadr"    SOURCES "test-netadr.c"     WIN32_LIBS "Ws2_32")
add_core_test(NAME "test-console"   SOURCES "test-console.c"    LINUX_LIBS "tinfo")
add_core_test(NAME "test-library"   SOURCES "test-library.c"    LINUX_LIBS "dl")
