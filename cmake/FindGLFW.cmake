find_path(GLFW_INCLUDE_DIR
    NAMES
        GLFW/glfw3.h)

find_library(GLFW_LIBRARIES
    NAMES
        glfw3
        glfw)

mark_as_advanced(GLFW_INCLUDE_DIR GLFW_glfw_LIBRARY)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(GLFW
    REQUIRED_VARS 
        GLFW_INCLUDE_DIR 
        GLFW_LIBRARIES)

if(GLFW_FOUND AND NOT TARGET glfw::glfw)
  add_library(glfw::glfw INTERFACE IMPORTED)
  
  target_include_directories(glfw::glfw
      INTERFACE ${GLFW_INCLUDE_DIR})

  target_link_libraries(glfw::glfw
      INTERFACE ${GLFW_LIBRARIES})
endif()
