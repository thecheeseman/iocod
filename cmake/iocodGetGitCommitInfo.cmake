# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

# if we're building from a git clone, we can grab the
# branch/commit hash and add this to the version string
# otherwise, these will be blank
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

execute_process(
    COMMAND sh -c "git log --oneline | wc -l | sed -e \"s/[ \t]*//g\""
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_COUNT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
