#---------------------------CMake Project Config-------------------------------
cmake_minimum_required(VERSION 3.10)

project(MandelLandscape LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#---------------------------Compilation database generation--------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_VERSION VERSION_LESS 3.12)
    file(APPEND ${CMAKE_BINARY_DIR}/compile_commands.json "")
else()
    file(TOUCH ${CMAKE_BINARY_DIR}/compile_commands.json)
endif()

configure_file(${CMAKE_BINARY_DIR}/compile_commands.json 
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    COPYONLY)

#---------------------------git submodule setup--------------------------------
option(UPDATE_SUBMODULES "Update git submodules before configuration" ON)

if(UPDATE_SUBMODULES)
  include(UpdateSubmodules)
endif()

#---------------------------CCache setup---------------------------------------
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

#---------------------------Threading library----------------------------------
find_package(Threads REQUIRED)

#---------------------------Glad-----------------------------------------------
add_subdirectory(extern/glad)

#---------------------------glm------------------------------------------------
if(NOT CMAKE_CROSSCOMPILING)
  find_package(glm CONFIG QUIET)
endif()

if(NOT glm_FOUND)
  add_subdirectory(extern/glm)
endif()

if(NOT TARGET glm::glm)
  add_library(glm::glm INTERFACE IMPORTED)

  target_link_libraries(glm::glm
      INTERFACE glm)
endif()

#---------------------------glfw-----------------------------------------------
if(NOT CMAKE_CROSSCOMPILING)
  find_package(GLFW QUIET)
endif()

if(NOT GLFW_FOUND)
  option(BUILD_SHARED_LIBS OFF)
  option(GLFW_BUILD_DOCS OFF)
  option(GLFW_INSTALL OFF)

  add_subdirectory(extern/glfw)
endif()

if(NOT TARGET glfw::glfw)
  add_library(glfw::glfw INTERFACE IMPORTED)

  target_link_libraries(glfw::glfw
      INTERFACE glfw)
endif()

#---------------------------stb------------------------------------------------
if(NOT CMAKE_CROSSCOMPILING)
  find_package(stb QUIET)
endif()

if(NOT TARGET stb::stb)
  add_library(stb::stb INTERFACE IMPORTED)

  if(stb_FOUND)
    target_link_libraries(stb::stb
        INTERFACE stb)
  else()
    target_include_directories(stb::stb
        INTERFACE extern/stb)
  endif()
endif()

#---------------------------MandelLandscape------------------------------------

add_subdirectory(src)
