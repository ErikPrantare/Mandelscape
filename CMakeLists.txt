cmake_minimum_required(VERSION 3.10)

project(MandelLandscape CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/build/compile_commands.json 
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    COPYONLY)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG(-Wall WALL)
if(WALL)
  add_definitions("-Wall")
endif()

CHECK_CXX_COMPILER_FLAG(-Werror WERROR)
if(WERROR)
  add_definitions("-Werror")
endif()

CHECK_CXX_COMPILER_FLAG(-Wextra WEXTRA)
if(WEXTRA)
  add_definitions("-Wextra")
endif()

CHECK_CXX_COMPILER_FLAG(-Wno-unused-parameter WNOUNUSEDPARAMETER)
if(WNOUNUSEDPARAMETER)
  add_definitions("-Wno-unused-parameter")
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/src/ SRC)

add_executable(app ${SRC})

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
  ${OPENGL_INCLUDE_DIRS} 
  ${GLUT_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS})
target_link_libraries(app 
  ${OPENGL_gl_LIBRARY} 
  ${GLUT_LIBRARY}
  ${GLEW_LIBRARIES})
