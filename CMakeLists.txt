cmake_minimum_required (VERSION 2.6)
project (Bobetex)

INCLUDE(conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

add_executable(btx ${SOURCE})
target_link_libraries(btx ${CONAN_LIBS})

add_custom_command(
	TARGET btx POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/build/bin" "${PROJECT_SOURCE_DIR}/bin"
)
