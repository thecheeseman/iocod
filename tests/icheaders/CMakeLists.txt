set(sources
    "ic_memory.c" 
    "strings/snprintfz.c" 
    "strings/strcasecmp.c" 
    "strings/strdup.c"  
    "strings/strlwr.c" 
    "strings/strncasecmp.c" 
    "strings/strncatz.c" 
    "strings/strncpyz.c" 
    "strings/strndup.c" 
    "strings/strupr.c")

add_executable(test_ic_headers
    "test.c" ${sources})
target_compile_options(test_ic_headers PRIVATE -DHAVE_CONFIG_H)

add_library(test_ic ${sources})

enable_testing()
add_subdirectory(tests)
