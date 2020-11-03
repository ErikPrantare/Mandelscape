file(GLOB SRC 
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

add_executable(Mandelscape)

target_sources(Mandelscape
    PRIVATE main.cpp)


if(MINGW) 
    target_link_libraries(Mandelscape
        PRIVATE -static
        PRIVATE MandelInterface)
else()
    target_link_libraries(Mandelscape
        PRIVATE MandelInterface)
endif()

set_target_properties(Mandelscape
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE)
        
if(CMAKE_BUILD_TYPE STREQUAL Release)
    set_target_properties(Mandelscape
        PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(CLANG_TIDY)
    set_target_properties(Mandelscape PROPERTIES
        CXX_CLANG_TIDY "clang-tidy;-header-filter=./src")
endif()

if(MSVC)
    target_compile_options(Mandelscape
        PRIVATE /W4)
else()
    target_compile_options(Mandelscape
        PRIVATE -Wall
        PRIVATE -Werror
        PRIVATE -Wextra)
endif()

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL Release)
    target_link_options(Mandelscape
        PRIVATE /SUBSYSTEM:WINDOWS
        PRIVATE /ENTRY:mainCRTStartup)
endif()
