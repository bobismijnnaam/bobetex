cmake_minimum_required (VERSION 2.6)
project (Bobetex)

# Get all the source files
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

# Define output and libraries
add_executable(btx ${SOURCE})
include_directories("${PROJECT_SOURCE_DIR}/src")

# TODO: Static crt?

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  target_link_libraries(btx GL SDL2)
  include_directories("/usr/include/SDL2")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  target_link_libraries(btx opengl32 SDL2main SDL2)
  include_directories("${PROJECT_SOURCE_DIR}\SDL2-2.0.8-win\include")
  link_directories("${PROJECT_SOURCE_DIR}\SDL2-2.0.8-win\lib\x64")
endif()

# Indicate that GLEW should be linked statically
add_definitions(-DGLEW_STATIC)
add_definitions(-std=c++11)
