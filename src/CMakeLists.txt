set(CMAKE_CXX_STANDARD 17)

file(GLOB 
    SRC
    "*.h"
    "*.cpp"
    "*.c")

add_executable(MandelLandscape
    ${SRC})

target_include_directories(MandelLandscape
    PRIVATE "../include")

target_link_libraries(MandelLandscape
    PRIVATE Threads::Threads
    PRIVATE glm::glm
    PRIVATE stb
    PRIVATE glfw)

target_compile_options(MandelLandscape
  #PRIVATE -O0
  #PRIVATE -g
    PRIVATE -Wall
    PRIVATE -Werror
    PRIVATE -Wextra
    PRIVATE -Wno-unused-parameter
    PRIVATE -Wno-switch)
