include(ExternalProject)

#---------------------------CMake Project Config-------------------------------
if(WIN32)
    cmake_minimum_required(VERSION 3.15)
    cmake_policy(SET CMP0091 NEW)
else()
    cmake_minimum_required(VERSION 3.12)
endif()

project(Mandelscape LANGUAGES CXX)

if(NOT CMAKE_MSVC_RUNTIME_LIBRARY)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(CLANG_TIDY "Run clang-tidy on Mandelscape" OFF)
option(BUILD_TESTING OFF)

#---------------------------Compilation database generation--------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(TOUCH ${CMAKE_BINARY_DIR}/compile_commands.json)
file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)

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
#find_package(glm CONFIG QUIET)

if(NOT glm_FOUND)
  add_subdirectory(extern/glm)
endif()

if(NOT TARGET glm::glm)
  add_library(glm::glm INTERFACE IMPORTED)

  target_link_libraries(glm::glm
      INTERFACE glm)
endif()

#---------------------------glfw-----------------------------------------------
#find_package(GLFW QUIET)

if(NOT GLFW_FOUND)
  option(BUILD_SHARED_LIBS OFF)
  option(GLFW_BUILD_DOCS OFF)
  option(GLFW_INSTALL OFF)
  option(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)

  add_subdirectory(extern/glfw)
endif()

if(NOT TARGET glfw::glfw)
  add_library(glfw::glfw INTERFACE IMPORTED)

  target_link_libraries(glfw::glfw
      INTERFACE glfw)
endif()

#---------------------------stb------------------------------------------------
#find_package(stb QUIET)

if(NOT TARGET stb::stb)
  add_library(stb::stb INTERFACE IMPORTED)

  if(stb_FOUND)
    target_include_directories(stb::stb
        INTERFACE ${stb_INCLUDE_DIR})
  else()
    target_include_directories(stb::stb
        INTERFACE extern/stb)
  endif()
endif()

#---------------------------luajit---------------------------------------------
#ExternalProject_Add(luajit
    #SOURCE_DIR ${PROJECT_SOURCE_DIR}/extern/luajit-2.0
    #BUILD_IN_SOURCE 1
    #BUILD_COMMAND make clean && make HOST_CC="/usr/bin/gcc -m64" CROSS="i686-w64-mingw32-" TARGET_SYS="Windows"
    #DOWNLOAD_COMMAND ""
    #CONFIGURE_COMMAND ""
    #INSTALL_COMMAND ""
#)

add_library(luajit::luajit INTERFACE IMPORTED)
target_link_libraries(luajit::luajit
    INTERFACE "${PROJECT_SOURCE_DIR}/extern/luajit-2.0/src/libluajit.a"
)

#---------------------------Catch2---------------------------------------------
if(BUILD_TESTING)
    find_package(Catch2 QUIET)

    if(NOT Catch2_FOUND)
        add_subdirectory(${PROJECT_SOURCE_DIR}/extern/Catch2)
    endif()
endif()

#---------------------------tests----------------------------------------------
if(BUILD_TESTING)
    option(COVERAGE "Run gcovr after testing" ON)
    option(GCOVR_GEN_HTML "Generate html files from coverage" OFF)

    add_subdirectory(tests)
endif()

#---------------------------Mandelscape----------------------------------------

add_subdirectory(src)
