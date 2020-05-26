if(NOT CMAKE_CROSSCOMPILING)
    find_package(Catch2 QUIET)
endif()

if(NOT Catch2_FOUND)
    add_subdirectory(${PROJECT_SOURCE_DIR}/extern/Catch2)
endif()

add_executable(tests)

target_sources(tests
    PRIVATE tests.cpp)

#string(CONCAT CLANG_TIDY_ARGS
    #"--header-filter=src/;"
    #"--warnings-as-errors=*;")
        ##"(${PROJECT_SOURCE_DIR}/src/*|"
        ##"${PROJECT_SOURCE_DIR}/tests/*);"

set_target_properties(tests
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE
        INTERPROCEDURAL_OPTIMIZATION TRUE
        CXX_CLANG_TIDY "clang-tidy;--warnings-as-errors=*")

target_link_libraries(tests
    PRIVATE Catch2::Catch2
    PRIVATE MandelInterface)
