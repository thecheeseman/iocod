add_library(ic_modules SHARED "syscalls.c" "modules.c")
target_link_libraries(ic_modules PUBLIC test_ic dl)

add_executable(test_ic_modules "test_modules.c" "syscalls.c" "modules.c")
target_link_libraries(test_ic_modules PUBLIC ic_modules)

add_library(testmod MODULE "m_syscalls.c" "m_example.c")
add_library(testmod2 MODULE "m_syscalls.c" "m_example2.c")
