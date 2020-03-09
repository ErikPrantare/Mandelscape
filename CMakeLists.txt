cmake_minimum_required(VERSION 2.4)

project(MandelLandscape)

file(GLOB src "*.h" "*.cpp")

add_definitions("-Wall -Werror -std=c++17")

add_executable(app main.cpp ${src})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIRS} 
    ${GLUT_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS})
target_link_libraries(app 
    ${OPENGL_LIBRARIES} 
    ${GLUT_LIBRARY}
    ${GLEW_LIBRARIES})
