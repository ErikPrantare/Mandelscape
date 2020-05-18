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
add_subdirectory(extern)

#--------------------------------Glad------------------------------------------
add_subdirectory(extern/glad)

#---------------------------glm------------------------------------------------
if(NOT CMAKE_CROSSCOMPILING)
  find_package(glm CONFIG QUIET)
endif()

if(NOT glm_FOUND)
  add_subdirectory(extern/glm)
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

find_package(Threads REQUIRED)

#---------------------------stb------------------------------------------------
if(NOT CMAKE_CROSSCOMPILING)
  find_package(stb QUIET)
endif()

if(NOT stb_FOUND)
    add_library(stb INTERFACE IMPORTED)

    target_include_directories(stb
        INTERFACE extern/stb)
endif()

#---------------------------MandelLandscape------------------------------------

add_subdirectory(src)
