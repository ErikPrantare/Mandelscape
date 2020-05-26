if(NOT CMAKE_CROSSCOMPILING)
    find_package(Catch2 QUIET)
endif()

if(NOT Catch2_FOUND)
    add_subdirectory(${PROJECT_SOURCE_DIR}/extern/Catch2)
endif()

add_executable(tests)

target_sources(tests
    PRIVATE tests.cpp)

set_target_properties(tests
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE
        INTERPROCEDURAL_OPTIMIZATION TRUE)

target_link_libraries(tests
    PRIVATE Catch2::Catch2
    PRIVATE MandelInterface)
