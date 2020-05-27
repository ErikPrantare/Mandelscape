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

target_compile_options(tests
    PRIVATE -ftest-coverage)

#include(CTest)
#include(Catch)
#catch_discover_tests(tests
    #WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_command(TARGET tests
    POST_BUILD
    COMMAND tests --use-colour yes)
