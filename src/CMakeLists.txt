file(GLOB 
    SRC "*.cpp")

add_executable(MandelLandscape
    ${SRC})

set_target_properties(MandelLandscape
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED
        INTERPROCEDURAL_OPTIMIZATION TRUE)

target_link_libraries(MandelLandscape
    PRIVATE Threads::Threads
    PRIVATE glad::glad
    PRIVATE glfw::glfw
    PRIVATE glm::glm
    PRIVATE stb::stb)

target_compile_options(MandelLandscape
    PRIVATE -Wall
    PRIVATE -Werror
    PRIVATE -Wextra
    PRIVATE -Wno-unused-parameter)
