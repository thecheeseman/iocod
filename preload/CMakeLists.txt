add_library(preload SHARED "")

set_target_properties(preload PROPERTIES
	POSITION_INDEPENDENT_CODE 1
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
	LIBRARY_OUTPUT_NAME "preload"
)

target_sources(preload PRIVATE
	"backtrace.c"
	"preload.c"
)

add_custom_command(
	TARGET preload POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy 
		"${CMAKE_CURRENT_BINARY_DIR}/libpreload.so"
		"${CMAKE_CURRENT_BINARY_DIR}/devpreload.so"
		COMMENT "Rename 'libpreload' to 'devpreload'"
)

add_custom_command(
	TARGET preload POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E remove
		"${CMAKE_CURRENT_BINARY_DIR}/libpreload.so"
		COMMENT "Remove original 'libpreload'"
)