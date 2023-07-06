# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(IOCOD_SOURCES
    # null implementations
    NullImplementation/NullClient.cpp
    NullImplementation/NullGame.cpp
    NullImplementation/NullServer.cpp
)

if (WIN32)
    list(APPEND IOCOD_SOURCES
        MainWindows.cpp
    )
elseif(APPLE)
    list(APPEND IOCOD_SOURCES
        MainMacOS.mm
    )
else()
    list(APPEND IOCOD_SOURCES
        MainLinux.cpp
    )
endif()

list(TRANSFORM IOCOD_SOURCES PREPEND "Source/")

#
# iocod executable
#
add_executable(iocod ${IOCOD_SOURCES})
add_dependencies(iocod iocodEngine)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(iocod PROPERTIES WIN32_EXECUTABLE TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set_target_properties(iocod PROPERTIES MACOSX_BUNDLE TRUE)
endif()

target_link_libraries(iocod PRIVATE iocodOptions iocodWarnings iocodIncludes)
target_link_libraries(iocod PRIVATE iocodEngine)
target_include_directories(iocod PRIVATE "Source/" "Include/")

# remove the "lib" prefix from the library name
set_target_properties(iocod PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
)

if (WIN32)
    add_custom_command(
        TARGET iocod
        POST_BUILD
        COMMENT "Embedding manifest file in iocod exe"
        WORKING_DIRECTORY $<TARGET_FILE_DIR:iocod>
        COMMAND "mt.exe"
            -manifest "${CMAKE_CURRENT_SOURCE_DIR}/iocod.manifest"
            -outputresource:$<TARGET_FILE:iocod>\;\#1
    )
endif()

add_custom_command(
    TARGET iocod
    POST_BUILD
    COMMENT "Generate sha512 checksum for iocod"
    WORKING_DIRECTORY $<TARGET_FILE_DIR:iocod>
    COMMAND ${CMAKE_COMMAND} -E sha512sum $<TARGET_FILE_NAME:iocod> > $<TARGET_FILE_NAME:iocod>.sha512
    VERBATIM
)
