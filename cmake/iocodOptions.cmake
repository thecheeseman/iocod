# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

message(STATUS "iocod Configuration:")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
message(STATUS "Enable warnings as errors: ${WARNINGS_AS_ERRORS}")

option(ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
message(STATUS "Enable address santizier: ${ENABLE_SANITIZER_ADDRESS}")

option(ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
message(STATUS "Enable leak sanitizer: ${ENABLE_SANITIZER_LEAK}")

option(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable undefined behavior sanitizer" OFF)
message(STATUS "Enable undefined behavior sanitizer: ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}")

option(ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
message(STATUS "Enable thread sanitizer: ${ENABLE_SANITIZER_THREAD}")

option(ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
message(STATUS "Enable memory sanitizer: ${ENABLE_SANITIZER_MEMORY}")

option(ENABLE_INTERPROCEDURAL_OPTIMIZATION "Enable interprocedural optimization" ON)
message(STATUS "Enable interprocedural optimization: ${ENABLE_INTERPROCEDURAL_OPTIMIZATION}")

option(ENABLE_UNITY_BUILD "Enable unity build" OFF)
message(STATUS "Enable unity build: ${ENABLE_UNITY_BUILD}")

option(ENABLE_DOXYGEN "Enable doxygen" OFF)
message(STATUS "Enable doxygen: ${ENABLE_DOXYGEN}")

option(BUILD_TESTS "Build tests" OFF)
message(STATUS "Build tests: ${BUILD_TESTS}")

option(BUILD_PLUGIN_CRYPTO "Build ScriptCrypto plugin" OFF)
message(STATUS "Build ScriptCrypto plugin: ${BUILD_PLUGIN_CRYPTO}")

option(BUILD_PLUGIN_MYSQL "Build ScriptMySQL plugin" OFF)
message(STATUS "Build ScriptMySQL plugin: ${BUILD_PLUGIN_MYSQL}")

option(BUILD_PLUGIN_SQLITE3 "Build ScriptSQLite3 plugin" OFF)
message(STATUS "Build ScriptSQLite3 plugin: ${BUILD_PLUGIN_SQLITE3}")

option(BUILD_32BIT "Build 32-bit" OFF)
message(STATUS "Build 32-bit: ${BUILD_32BIT}")

option(USE_MOLD_LINKER "Use mold as linker instead of system default" OFF)
message(STATUS "Use mold linker: ${USE_MOLD_LINKER}")

list(POP_BACK CMAKE_MESSAGE_INDENT)

if (BUILD_32BIT AND APPLE)
    message(FATAL_ERROR "32-bit builds are not supported on macOS")
endif()
