find_path(GLFW_INCLUDE_DIR
    NAMES
        GLFW/glfw3.h
    PATHS
        ${GLFW_LOCATION}/include
        $ENV{GLFW_LOCATION}/include
        $ENV{PROGRAMFILES}/GLFW/include
        ${OPENGL_INCLUDE_DIR}
        /usr/include
        /usr/local/include
        /usr/include/GL)

find_library(GLFW_glfw_LIBRARY
    NAMES
        glfw3
        glfw
    PATHS
        ${GLFW_LOCATION}/lib
        ${GLFW_LOCATION}/lib/x64
        $ENV{GLFW_LOCATION}/lib
        $ENV{GLFW_LOCATION}/lib/x64
        $ENV{PROGRAMFILES}/GLFW/lib
        ${OPENGL_LIBRARY_DIR}
        /usr/lib
        /usr/local/lib
        /usr/lib64
        /usr/local/lib64)

if(GLFW_INCLUDE_DIR AND GLFW_glfw_LIBRARY)
  if(UNIX)
    find_package(X11 REQUIRED)
  endif()

  set(GLFW_INCLUDE_DIR
      ${GLFW_INCLUDE_DIR}
      ${X11_INCLUDE_DIR})

  set(GLFW_LIBRARIES
      ${GLFW_glfw_LIBRARY}
      ${X11_LIBRARY})

  set(GLFW_LIBRARY
      ${GLFW_LIBRARIES})
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLFW
    REQUIRED_VARS 
        GLFW_INCLUDE_DIR 
        GLFW_LIBRARIES)

mark_as_advanced(
    GLFW_INCLUDE_DIR
    GLFW_glfw_LIBRARY
    GLFW_LIBRARIES)
