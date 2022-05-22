project(libconfuse)

include(CheckStdCHeaders)
include(CheckIncludeFiles)
include(CheckSymbolExists)

check_stdc_headers()

#dcgettext()
check_include_files(dlfcn.h HAVE_DLFCN_H)
# gettext()
# iconv()

check_include_files(memory.h HAVE_MEMORY_H)

check_symbol_exists(string.h "strdup" HAVE_STRDUP)
check_symbol_exists(string.h "strndup" HAVE_STRNDUP)
check_include_files(strings.h HAVE_STRINGS_H)
if (HAVE_STRINGS_H)
    check_symbol_exists(strings.h "strcasecmp" HAVE_STRCASECMP)
endif()

check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(windows.h HAVE_WINDOWS_H)

check_symbol_exists(dcgettext "libintl.h" HAVE_DCGETTEXT)
check_symbol_exists(gettext "libintl.h" HAVE_GETTEXT)
check_symbol_exists(iconv "iconv.h" HAVE_ICONV)

check_symbol_exists(funopen "stdio.h" HAVE_FUNOPEN)
check_symbol_exists(fmemopen "stdio.h" HAVE_FMEMOPEN)
check_symbol_exists(reallocarray "stdlib.h" HAVE_REALLOCARRAY)
check_symbol_exists(setenv "stdlib.h" HAVE_SETENV)
check_symbol_exists(unsetenv "stdlib.h" HAVE_UNSETENV)
check_symbol_exists(_putenv "stdlib.h" HAVE__PUTENV)

configure_file("config.h.in" "${PROJECT_BINARY_DIR}/confuse_config.h")
include_directories(${PROJECT_BINARY_DIR})
add_compile_options(-DHAVE_CONFIG_H -DBUILDING_STATIC)

find_package(FLEX)
FLEX_TARGET(lexer src/lexer.l src/lexer.c COMPILE_FLAGS -Pcfg_yy)

set(CONFUSE_SRCS "src/lexer.c" "src/confuse.c")

if (NOT HAVE_FMEMOPEN)
    list(APPEND CONFUSE_SRCS "src/fmemopen.c")
endif()

if (NOT HAVE_REALLOCARRAY)
    list(APPEND CONFUSE_SRCS "src/reallocarray.c")
endif()

add_library(confuse STATIC "src/lexer.c" "src/confuse.c")
