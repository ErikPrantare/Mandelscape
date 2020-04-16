find_package(Git REQUIRED)

option(GIT_SUBMODULE "Check submodules during build" ON)

if(GIT_SUBMODULE)
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
endif()

set(OpenGL_GL_PREFERENCE LEGACY)

option(ONLY_LIBS ON)
option(glew-cmake_BUILD_SHARED OFF)
add_subdirectory(glew-cmake)

add_subdirectory(glfw)

add_subdirectory(glm)

add_library(stb INTERFACE)
target_include_directories(stb
    INTERFACE stb)
