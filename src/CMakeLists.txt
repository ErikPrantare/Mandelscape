set(CMAKE_CXX_STANDARD 17)

file(GLOB 
    SRC
    "*.h"
    "*.cpp")

add_executable(MandelLandscape
    ${SRC})

target_link_libraries(MandelLandscape
    PRIVATE libglew_static
    PRIVATE Threads::Threads
    PRIVATE glm::glm
    PRIVATE stb
    PRIVATE glfw)

target_compile_options(MandelLandscape
    PRIVATE -Wall
    PRIVATE -Werror
    PRIVATE -Wextra
    PRIVATE -Wno-unused-parameter
    PRIVATE -Wno-switch)
