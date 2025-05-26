cmake_minimum_required(VERSION 3.5)
project(neural_ants LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add debug flags
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

include(FetchContent)

FetchContent_Declare(
        FMT_CXX
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.1.4
)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)

FetchContent_MakeAvailable(FMT_CXX)
FetchContent_MakeAvailable(raylib)

# Separate C and C++ sources
set(C_SOURCES
    src/raylibdrawex.c
    src/raylibmathex.c
)

set(CXX_SOURCES
    src/main.cpp
    src/game.cpp 
    src/input.cpp
    src/world.cpp
    src/ant.cpp
    src/population.cpp
    src/resources.cpp
    src/neuron.cpp
    src/neural_network.cpp
    src/genome.cpp
    src/surroundings.cpp
    src/food.cpp
    src/brain.cpp
)

add_executable(neural_ants ${C_SOURCES} ${CXX_SOURCES})

target_link_libraries(neural_ants PRIVATE raylib fmt::fmt)

# Add include directories
target_include_directories(neural_ants PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
