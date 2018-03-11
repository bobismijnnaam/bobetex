cmake_minimum_required (VERSION 2.6)
project (Bobetex)

# Get conan library info
# TODO: Include more libraries as conan grows (glew, duktape, stb)
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# CONAN_BASIC_SETUP()

# Get all the source files
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

# Define output and libraries
add_executable(btx ${SOURCE})
target_link_libraries(btx GL SDL2 ${CONAN_LIBS})
include_directories("${PROJECT_SOURCE_DIR}/src" "/usr/include/SDL2")
set(CMAKE_BUILD_TYPE Debug)

# Indicate that GLEW should be linked statically
add_definitions(-DGLEW_STATIC)
add_definitions(-std=c++11)

# Copy all resulting binaries
add_custom_command(
	TARGET btx POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/build/bin" "${PROJECT_SOURCE_DIR}/bin"
)
