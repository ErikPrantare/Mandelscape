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
