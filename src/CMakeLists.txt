set(CMAKE_CXX_STANDARD 17)

file(GLOB 
    SRC
    "*.cpp")

add_executable(MandelLandscape
    ${SRC})

target_link_libraries(MandelLandscape
    PRIVATE Threads::Threads
    PRIVATE glad::glad
    PRIVATE glfw
    PRIVATE glm
    PRIVATE stb)

target_compile_options(MandelLandscape
    PRIVATE -Wall
    PRIVATE -Werror
    PRIVATE -Wextra
    PRIVATE -Wno-unused-parameter)
