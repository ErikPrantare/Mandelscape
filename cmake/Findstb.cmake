find_path(STB_INCLUDE_DIR
    NAMES
        stb_image.h
    PATHS
        ${STB_LOCATION}
        $ENV{STB_LOCATION}
        $ENV{PROGRAMFILES}/stb
        /usr/include
        /usr/local/include
        /usr/include/stb
        /usr/local/include/stb)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(stb
    REQUIRED_VARS 
        STB_INCLUDE_DIR)

if(NOT TARGET stb)
  add_library(stb INTERFACE IMPORTED)

  target_include_directories(stb
      INTERFACE ${STB_INCLUDE_DIR})
endif()

mark_as_advanced(
    STB_INCLUDE_DIR)
