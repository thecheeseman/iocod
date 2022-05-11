add_executable(prototypegen)
target_sources(prototypegen PRIVATE
	"prototypegen.c" 
	"../src/parse.c"
	"../src/shared.c"
	"testlib.c")
set_target_properties(prototypegen PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/prototypegen/"
)