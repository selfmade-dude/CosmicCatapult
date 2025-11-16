# Cosmic Catapult main CMake configuration
cmake_minimum_required(VERSION 3.21)

project(CosmicCatapult
    VERSION 0.1.0
    LANGUAGES CXX
)

# ----------------------------
# C++ standard configuration
# ----------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------
# Qt setup
# ----------------------------

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

#Enable Qt's automatic processing for moc, uic, rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ----------------------------
# Compiler warnings (light)
# ----------------------------

if (MSVC) 
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ----------------------------
# Project options
# ----------------------------

option(BUILD_TESTING "Build the test suite" ON)

# ----------------------------
# Subdirectories (modules)
# ----------------------------

add_subdirectory(core)
add_subdirectory(sim)
add_subdirectory(io)
add_subdirectory(app)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()