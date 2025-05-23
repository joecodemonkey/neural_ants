cmake_minimum_required(VERSION 3.5)
project(neural_ants)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

add_executable(neural_ants main.cpp world.cpp ant.cpp population.cpp  resources.cpp
        resources.hpp neuron.cpp neural_network.cpp genome.cpp surroundings.cpp)

target_link_libraries(neural_ants PRIVATE raylib fmt::fmt)
target_include_directories(neural_ants PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
)
