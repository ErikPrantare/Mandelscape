cmake_minimum_required(VERSION 3.10)

project(MandelLandscape LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_VERSION VERSION_LESS 3.12)
    file(APPEND ${CMAKE_BINARY_DIR}/compile_commands.json "")
else()
    file(TOUCH ${CMAKE_BINARY_DIR}/compile_commands.json)
endif()

configure_file(${CMAKE_BINARY_DIR}/compile_commands.json 
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    COPYONLY)

add_subdirectory(extern)

find_package(Threads REQUIRED)

add_subdirectory(src)
