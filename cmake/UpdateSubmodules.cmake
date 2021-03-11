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

find_package(Git REQUIRED)

message(STATUS "Submodule update")
execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMOD_RESULT)

if(NOT GIT_SUBMOD_RESULT EQUAL "0")
    message(FATAL_ERROR 
        "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, "
        "please checkout submodules")
endif()
