set(CMAKE_CXX_STANDARD 17)

file(GLOB 
    SRC
    "*.h"
    "*.cpp")

add_executable(MandelLandscape
    ${SRC})

add_dependencies(MandelLandscape
    freeglut
    libglew_shared)

target_include_directories(MandelLandscape
    PRIVATE freeglut
    PRIVATE libglew_shared)

target_link_libraries(MandelLandscape
    PRIVATE freeglut
    PRIVATE libglew_shared
    PRIVATE Threads::Threads)

target_compile_options(MandelLandscape
    PRIVATE -Wall
    PRIVATE -Werror
    PRIVATE -Wextra
    PRIVATE -Wno-unused-parameter)
