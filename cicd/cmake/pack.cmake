message("==>> Configuring...")
execute_process( # configuration
	COMMAND "${CMAKE_COMMAND}" $ENV{CONFIGURATION_OPTIONS} -B build -S . 
	-DCMAKE_BUILD_TYPE=Release
	WORKING_DIRECTORY "$ENV{REPO}"
	COMMAND_ERROR_IS_FATAL ANY
)

file(MAKE_DIRECTORY "$ENV{REPO}/build")

message("==>> Building...")
execute_process( # building
	COMMAND "${CMAKE_COMMAND}" --build build
	WORKING_DIRECTORY "$ENV{REPO}"
	COMMAND_ERROR_IS_FATAL ANY
)

message("==>> Packaging for ${CMAKE_SYSTEM_NAME}...")
execute_process( # test 
	COMMAND "${CMAKE_CPACK_COMMAND}"
	WORKING_DIRECTORY "$ENV{REPO}/build"
	RESULT_VARIABLE CODE
)
