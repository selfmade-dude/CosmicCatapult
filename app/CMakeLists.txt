# App CMakeLists.txt - builds the main Qt executable

# Create the executable target for the GUI application
add_executable(cosmic_catapult_app
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

# Ensure the target can find headers in this directory
target_include_directories(cosmic_catapult_app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link the executable with the required Qt libraries
target_link_libraries(cosmic_catapult_app
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        cosmic_core
)