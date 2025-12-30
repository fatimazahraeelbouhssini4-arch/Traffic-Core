cmake_minimum_required(VERSION 3.10)
project(SimulationVehicules)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/Vehicule.cpp
    src/Truck.cpp
    src/Bus.cpp
    src/Car.cpp
    src/VehiculeFactory.cpp
    src/Intersection.cpp
    src/Route.cpp
    src/RoadNetwork.cpp
    src/Pathfinder.cpp
    src/PathfindingStrategy.cpp
    src/Simulator.cpp
    src/ConfigLoader.cpp
    src/SimulationObserver.cpp
)

add_executable(simulation ${SOURCES})

target_link_libraries(simulation PRIVATE raylib)

if(WIN32)
    target_link_libraries(simulation PRIVATE winmm)
endif()