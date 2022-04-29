add_library(ic_modules SHARED "ic_modules.c")
target_link_libraries(ic_modules PUBLIC test_ic dl)

add_executable(test_ic_modules "test_ic_modules.c" "ic_modules.c")
target_link_libraries(test_ic_modules PUBLIC ic_modules)

add_library(testmod SHARED "testmod.c")
target_link_libraries(testmod PUBLIC ic_modules)
