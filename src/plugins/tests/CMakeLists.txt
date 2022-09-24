function(add_test_plugin)
    set(one_value_args NAME)
    set(multi_value_args SOURCES)
    cmake_parse_arguments(PARSE_ARGV 0 TP
        ""
        "${one_value_args}"
        "${multi_value_args}"
    )
    add_library(${TP_NAME} SHARED "${TP_NAME}/${TP_NAME}.cxx")
    if (DEFINED TP_SOURCES)
        foreach(SRC IN ITEMS ${TP_SOURCES})
            message(STATUS ${SRC})
            target_sources(${TP_NAME} PRIVATE "${TP_NAME}/${SRC}")
        endforeach()
    endif()
    
    target_include_directories(${TP_NAME} PRIVATE 
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/src/include"
    )
    set_target_properties(${TP_NAME} PROPERTIES
        PREFIX ""
        ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/bin/plugins"
        LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/bin/plugins"
        RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/bin/plugins"
    )
endfunction()

add_test_plugin(NAME bad_plugin)
add_test_plugin(NAME demo_plugin)
add_test_plugin(NAME old_api_plugin)
