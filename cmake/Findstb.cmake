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

find_path(stb_stb_INCLUDE_DIR
    NAMES
        stb/stb_image.h)

if(stb_stb_INCLUDE_DIR)
    set(stb_INCLUDE_DIR ${stb_stb_INCLUDE_DIR}/stb)
endif()

mark_as_advanced(stb_stb_INCLUDE_DIR stb_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(stb
    REQUIRED_VARS 
        stb_INCLUDE_DIR)
