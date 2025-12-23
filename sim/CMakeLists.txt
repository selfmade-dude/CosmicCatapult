add_library(cosmic_sim 
    SimulationModel.cpp
)

target_include_directories(cosmic_sim
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cosmic_sim
    PUBLIC
        cosmic_core
)