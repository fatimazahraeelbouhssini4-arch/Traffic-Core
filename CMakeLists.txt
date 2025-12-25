cmake_minimum_required(VERSION 3.16)

project(Traffic3D LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver raylib
find_package(raylib REQUIRED)

# Récupérer tous les .cpp
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# Ajouter l'exécutable (⚠️ main.cpp inclus)
add_executable(Traffic3D
    main.cpp
    ${SOURCES}
)

# Inclure les headers
target_include_directories(Traffic3D PRIVATE
    include
)

# Lier raylib
target_link_libraries(Traffic3D
    raylib
)
