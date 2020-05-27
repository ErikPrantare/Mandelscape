if(NOT CMAKE_CROSSCOMPILING)
    find_package(Catch2 QUIET)
endif()

if(NOT Catch2_FOUND)
    add_subdirectory(${PROJECT_SOURCE_DIR}/extern/Catch2)
endif()

add_executable(tests)

target_sources(tests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp)

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

add_custom_command(TARGET tests
    POST_BUILD
    COMMAND tests --use-colour yes
    COMMAND echo $<$<AND:$<BOOL:COVERAGE>,$<CONFIG:Debug>>:${GCOVR_CALL}> | sh
    COMMAND echo $<$<AND:$<BOOL:COVERAGE>,$<CONFIG:Debug>>:${GEN_HTML}> | sh)

target_link_options(tests
    PRIVATE $<$<AND:$<CONFIG:Debug>,$<BOOL:COVERAGE>>:-fprofile-arcs>)

target_compile_options(tests
    PRIVATE $<$<AND:$<CONFIG:Debug>,$<BOOL:COVERAGE>>:-fprofile-arcs>
    PRIVATE $<$<AND:$<CONFIG:Debug>,$<BOOL:COVERAGE>>:-ftest-coverage>)
