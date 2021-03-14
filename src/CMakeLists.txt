#[[ Project: Mandelscape
 [[ Copyright (C) 2020-2021 Erik Präntare
 [[
 [[ This program is free software: you can redistribute it and/or modify
 [[ it under the terms of the GNU Affero General Public License as published by
 [[ the Free Software Foundation, either version 3 of the License, or
 [[ (at your option) any later version.
 [[ 
 [[ This program is distributed in the hope that it will be useful,
 [[ but WITHOUT ANY WARRANTY; without even the implied warranty of
 [[ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 [[ GNU Affero General Public License for more details.
 [[ 
 [[ You should have received a copy of the GNU Affero General Public License
 [[ along with this program.  If not, see <http://www.gnu.org/licenses/>.
#]]

file(GLOB SRC 
    "*.cpp")

list(FILTER SRC EXCLUDE REGEX "main.cpp")

add_library(MandelInterface INTERFACE)

target_sources(MandelInterface
    INTERFACE ${SRC})

target_include_directories(MandelInterface
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
    INTERFACE ${PROJECT_SOURCE_DIR}/extern/luajit-2.0/src
)

target_link_libraries(MandelInterface
    INTERFACE Threads::Threads
    INTERFACE glad::glad
    INTERFACE glfw::glfw
    INTERFACE glm::glm
    INTERFACE stb::stb
    INTERFACE luajit::luajit
    INTERFACE nfde::nfde
)

add_executable(Mandelscape)

target_sources(Mandelscape
    PRIVATE main.cpp)


if(MINGW) 
    target_link_libraries(Mandelscape
        PRIVATE -static
        PRIVATE -mwindows
        PRIVATE MandelInterface)
else()
    target_link_libraries(Mandelscape
        PRIVATE MandelInterface)
endif()

set_target_properties(Mandelscape
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE)
        
if(CMAKE_BUILD_TYPE STREQUAL Release)
    set_target_properties(Mandelscape
        PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(CLANG_TIDY)
    set_target_properties(Mandelscape PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;-header-filter=./src")
endif()

if(MSVC)
    target_compile_options(Mandelscape
        PRIVATE /W4)
else()
    target_compile_options(Mandelscape
        PRIVATE -Wall
        PRIVATE -Werror
        PRIVATE -Wextra)
endif()

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL Release)
    target_link_options(Mandelscape
        PRIVATE /SUBSYSTEM:WINDOWS
        PRIVATE /ENTRY:mainCRTStartup)
endif()
