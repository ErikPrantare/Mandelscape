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

add_executable(tests)

target_sources(tests
    PRIVATE ${SRC})

set_target_properties(tests
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE)

target_link_libraries(tests
    PRIVATE Catch2::Catch2
    PRIVATE MandelInterface)

string(CONCAT GCOVR_CALL
    "gcovr -r ${PROJECT_SOURCE_DIR}/src "
    "--object-directory ${PROJECT_BINARY_DIR}")

if(GCOVR_GEN_HTML)
    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/coverage)

    string(CONCAT GEN_HTML
        "gcovr -r ${PROJECT_SOURCE_DIR}/src "
        "--object-directory ${PROJECT_BINARY_DIR} "
        "--html-details ${PROJECT_SOURCE_DIR}/coverage/coverage.html")
endif()


if(WIN32)
    add_custom_command(TARGET tests
        POST_BUILD
        COMMAND tests --use-colour yes)
else()
    set(COVERAGE_AND_DEBUG 
        "$<AND:$<BOOL:COVERAGE>,$<CONFIG:Debug>>")

    add_custom_command(TARGET tests
        POST_BUILD
        COMMAND tests --use-colour yes
        COMMAND echo $<${COVERAGE_AND_DEBUG}:${GCOVR_CALL}> | sh
        COMMAND echo $<${COVERAGE_AND_DEBUG}:${GEN_HTML}> | sh)

    target_link_options(tests
        PRIVATE $<${COVERAGE_AND_DEBUG}:-fprofile-arcs>)
    
    target_compile_options(tests
        PRIVATE $<${COVERAGE_AND_DEBUG}:-fprofile-arcs>
        PRIVATE $<${COVERAGE_AND_DEBUG}:-ftest-coverage>)
endif()
