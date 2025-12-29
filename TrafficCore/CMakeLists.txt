cmake_minimum_required(VERSION 3.16)
project(Traffic3D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver Raylib
find_package(raylib REQUIRED)

# Tous les .cpp
file(GLOB_RECURSE SOURCES src/*.cpp)

# Ajouter exécutable avec ton main réel
add_executable(Traffic3D
    demos/main.cpp
    ${SOURCES}
)

# Include headers
target_include_directories(Traffic3D PRIVATE include)

# Lier raylib + libs Windows
target_link_libraries(Traffic3D
    raylib
    opengl32
    gdi32
    winmm
    kernel32
)
