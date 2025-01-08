cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH 
	"${CMAKE_CURRENT_LIST_DIR}/cmake"
)

include(cmake-scripts/gitversion)
gitversion()

project(snake-cpp
	LANGUAGES CXX
	VERSION ${GIT_VERSION_TAG}
)

add_subdirectory(src)

add_subdirectory(packaging)
