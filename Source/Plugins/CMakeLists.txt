# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

function(add_plugin)
    set(one_value_args NAME)
    set(multi_value_args SOURCES INCLUDE_DIRS LINK_LIBRARIES)
    cmake_parse_arguments(PARSE_ARGV 0 PLUGIN
        ""
        "${one_value_args}"
        "${multi_value_args}"
    )
    
    # by default, all plugins should have the same name as their source file
    add_library(${PLUGIN_NAME} SHARED)

    # add the main source file (.c or .cpp)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_NAME}/${PLUGIN_NAME}.c")
        target_sources(${PLUGIN_NAME} PRIVATE "${PLUGIN_NAME}/${PLUGIN_NAME}.c")
    elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_NAME}/${PLUGIN_NAME}.cpp")
        target_sources(${PLUGIN_NAME} PRIVATE "${PLUGIN_NAME}/${PLUGIN_NAME}.cpp")
    else()
        message(FATAL_ERROR "No source file found for plugin ${PLUGIN_NAME}")
    endif()

    target_link_libraries(${PLUGIN_NAME} PRIVATE iocod_options iocod_warnings iocod_includes)

    # add any additional sources
    if (DEFINED PLUGIN_SOURCES)
        foreach(SRC IN ITEMS ${PLUGIN_SOURCES})
            target_sources(${PLUGIN_NAME} PRIVATE "${PLUGIN_NAME}/${SRC}")
        endforeach()
    endif()

    if (DEFINED PLUGIN_INCLUDE_DIRS)
        foreach(INC IN ITEMS ${PLUGIN_INCLUDE_DIRS})
            target_include_directories(${PLUGIN_NAME} PRIVATE "${INC}")
        endforeach()
    endif()

    if (DEFINED PLUGIN_LINK_LIBRARIES)
        foreach(LIB IN ITEMS ${PLUGIN_LINK_LIBRARIES})
            target_link_libraries(${PLUGIN_NAME} PRIVATE ${LIB})
        endforeach()
    endif()
    
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        PREFIX ""
        ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/Plugins"
        LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/Plugins"
        RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/Plugins"
    )
endfunction()

add_plugin(NAME TestPlugin)