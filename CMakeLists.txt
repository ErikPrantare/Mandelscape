cmake_minimum_required(VERSION 3.10...3.17)

project(MandelLandscape LANGUAGES CXX)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
