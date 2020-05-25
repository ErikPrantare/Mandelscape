find_path(STB_STB_INCLUDE_DIR
    NAMES
        stb/stb_image.h)

if(STB_STB_INCLUDE_DIR)
    set(STB_INCLUDE_DIR ${STB_STB_INCLUDE_DIR}/stb)
endif()

mark_as_advanced(STB_STB_INCLUDE_DIR STB_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(stb
    REQUIRED_VARS 
        STB_INCLUDE_DIR)
