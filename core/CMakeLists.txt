# Core CMakeLists.txt - builds the physics core library

# Create an interface library target for now (header-only skeleton)
add_library(cosmic_core INTERFACE)

# Expose this directory as an include path to users of cosmic_core
target_include_directories(cosmic_core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)