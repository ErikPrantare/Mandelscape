cmake_minimum_required(VERSION 3.10)

project(MandelLandscape LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_VERSION VERSION_LESS 3.12)
    file(APPEND ${CMAKE_BINARY_DIR}/compile_commands.json "")
else()
    file(TOUCH ${CMAKE_BINARY_DIR}/compile_commands.json)
endif()
configure_file(${CMAKE_BINARY_DIR}/compile_commands.json 
  compile_commands.json
  COPYONLY)

add_subdirectory(extern)

set(OpenGL_GL_PREFERENCE LEGACY)
add_subdirectory(extern/FreeGLUT/freeglut/freeglut)

add_subdirectory(extern/glew-cmake)

find_package(Threads REQUIRED)

add_subdirectory(src)
