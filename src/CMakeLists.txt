file(GLOB SRC 
    "*.hpp"
    "*.cpp")

list(FILTER SRC EXCLUDE REGEX "main.cpp")

add_library(MandelInterface INTERFACE)

target_sources(MandelInterface
    INTERFACE ${SRC})

target_include_directories(MandelInterface
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

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
        CXX_STANDARD_REQUIRED TRUE)
        
if(CMAKE_BUILD_TYPE STREQUAL Release)
    set_target_properties(MandelLandscape
        PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(CLANG_TIDY)
    set_target_properties(MandelLandscape PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;-header-filter=./src")
endif()

if(WIN32)
    target_compile_options(MandelLandscape
        PRIVATE /W4)

else()
    target_compile_options(MandelLandscape
        PRIVATE -Wall
        PRIVATE -Werror
        PRIVATE -Wextra)
endif()

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL Release)
    target_link_options(MandelLandscape
        PRIVATE /SUBSYSTEM:WINDOWS
        PRIVATE /ENTRY:mainCRTStartup)
endif()