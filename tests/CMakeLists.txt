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

add_custom_command(TARGET tests
    POST_BUILD
    COMMAND tests --use-colour yes)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    include(CodeCoverage)
    append_coverage_compiler_flags()

    setup_target_for_coverage_gcovr_html(
        NAME coverage
        EXECUTABLE tests
        BASE_DIRECTORY ${PROJECT_SOURCE_DIR}/src
        DEPENDENCIES tests)
endif()
