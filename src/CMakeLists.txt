set(CMAKE_CXX_STANDARD 17)

file(GLOB 
    SRC
    "*.h"
    "*.cpp")

add_executable(MandelLandscape
    ${SRC})

add_dependencies(MandelLandscape
    freeglut_static
    libglew_static)

target_include_directories(MandelLandscape
    PRIVATE freeglut_static
    PRIVATE libglew_static)

target_link_libraries(MandelLandscape
    PRIVATE freeglut_static
    PRIVATE libglew_static
    PRIVATE Threads::Threads)

target_compile_options(MandelLandscape
    PRIVATE -Wall
    PRIVATE -Werror
    PRIVATE -Wextra
    PRIVATE -Wno-unused-parameter)
