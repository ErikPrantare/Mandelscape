project(glad LANGUAGES C)

option(GLAD_BUILD_STATIC "Build glad as a static library" ON)

if(GLAD_BUILD_STATIC)
  add_library(glad_static STATIC)

  target_sources(glad_static
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)

  target_include_directories(glad_static
      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

  add_library(glad::glad ALIAS glad_static)

else()
  add_library(glad_dynamic DYNAMIC)
  
  target_sources(glad_dynamic
      PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)
 
  target_include_directories(glad_dynamic
      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

  add_library(glad::glad ALIAS glad_dynamic)

endif()
