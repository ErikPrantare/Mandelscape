file(GLOB 
    SRC "*.cpp")

list(FILTER SRC EXCLUDE REGEX "main.cpp")

add_library(MandelInterface INTERFACE)

target_sources(MandelInterface
    INTERFACE ${SRC})

target_link_libraries(MandelInterface
    INTERFACE Threads::Threads
    INTERFACE glad::glad
    INTERFACE glfw::glfw
    INTERFACE glm::glm
    INTERFACE stb::stb)

add_executable(MandelLandscape)

target_sources(MandelLandscape
    PRIVATE main.cpp)

target_link_libraries(MandelLandscape
    PRIVATE MandelInterface)

set_target_properties(MandelLandscape
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE
        INTERPROCEDURAL_OPTIMIZATION TRUE)


target_compile_options(MandelLandscape
    PRIVATE -Wall
    PRIVATE -Werror
    PRIVATE -Wextra
    PRIVATE -Wno-unused-parameter)
