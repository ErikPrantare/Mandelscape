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

find_path(GLFW_INCLUDE_DIR
    NAMES
        GLFW/glfw3.h)

find_library(GLFW_LIBRARIES
    NAMES
        glfw3
        glfw)

mark_as_advanced(GLFW_INCLUDE_DIR GLFW_glfw_LIBRARY)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLFW
    REQUIRED_VARS 
        GLFW_INCLUDE_DIR 
        GLFW_LIBRARIES)

if(GLFW_FOUND AND NOT TARGET glfw::glfw)
  add_library(glfw::glfw INTERFACE IMPORTED)
  
  target_include_directories(glfw::glfw
      INTERFACE ${GLFW_INCLUDE_DIR})

  target_link_libraries(glfw::glfw
      INTERFACE ${GLFW_LIBRARIES})
endif()
