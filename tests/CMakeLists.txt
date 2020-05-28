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

set(COV_DEBUG_WIN 
    "$<AND:$<BOOL:COVERAGE>,$<CONFIG:Debug>,$<NOT:$<BOOL:${WIN32}>>>")

add_custom_command(TARGET tests
    POST_BUILD
    COMMAND tests --use-colour yes
    COMMAND $<$<NOT:${COV_DEBUG_WIN}>:exit>
    COMMAND echo $<${COV_DEBUG_WIN}:${GCOVR_CALL}> | sh
    COMMAND echo $<${COV_DEBUG_WIN}:${GEN_HTML}> | sh)

    target_link_options(tests
        PRIVATE $<${COV_DEBUG_WIN}:-fprofile-arcs>)
        
    target_compile_options(tests
        PRIVATE $<${COV_DEBUG_WIN}:-fprofile-arcs>
        PRIVATE $<${COV_DEBUG_WIN}:-ftest-coverage>)
