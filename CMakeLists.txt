cmake_minimum_required (VERSION 2.6)
project (Bobetex)

INCLUDE(conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

add_executable(btx ${SOURCE})
