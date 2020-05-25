find_path(stb_stb_INCLUDE_DIR
    NAMES
        stb/stb_image.h)

if(stb_stb_INCLUDE_DIR)
    set(stb_INCLUDE_DIR ${stb_stb_INCLUDE_DIR}/stb)
endif()

mark_as_advanced(stb_stb_INCLUDE_DIR stb_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(stb
    REQUIRED_VARS 
        stb_INCLUDE_DIR)
