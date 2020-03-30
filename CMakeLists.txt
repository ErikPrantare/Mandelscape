cmake_minimum_required(VERSION 3.10)

project(MandelLandscape LANGUAGES CXX)

if(CMAKE_VERSION VERSION_LESS 3.12)
    file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
else()
    file(TOUCH ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
endif()
configure_file(${CMAKE_BINARY_DIR}/compile_commands.json 
  ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  COPYONLY)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
