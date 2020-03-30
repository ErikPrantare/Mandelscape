set(CMAKE_CXX_STANDARD 17)

file(GLOB 
    SRC
    "*.h"
    "*.cpp")

add_executable(MandelLandscape
    ${SRC})

add_dependencies(MandelLandscape
    freeglut)

target_include_directories(MandelLandscape
    PRIVATE OpenGL::GL
    PRIVATE FreeGLUT::freeglut
    PRIVATE GLEW::glew)

target_link_libraries(MandelLandscape
    PRIVATE OpenGL::GL
    PRIVATE freeglut
    PRIVATE GLEW::GLEW
    PRIVATE Threads::Threads)

target_compile_options(MandelLandscape
    PRIVATE -Wall
    PRIVATE -Werror
    PRIVATE -Wextra
    PRIVATE -Wno-unused-parameter)
