project(glad LANGUAGES C)

option(GLAD_BUILD_STATIC "Build glad as a static library" ON)

if(GLAD_BUILD_STATIC)
  add_library(glad_static STATIC)

  set(GLAD_LIB_NAME glad_static)
else()
  add_library(glad_shared SHARED)

  set(GLAD_LIB_NAME glad_shared)
endif()

target_sources(${GLAD_LIB_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)
 
target_include_directories(${GLAD_LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${GLAD_LIB_NAME}
    PRIVATE ${CMAKE_DL_LIBS})

add_library(glad::glad ALIAS ${GLAD_LIB_NAME})
