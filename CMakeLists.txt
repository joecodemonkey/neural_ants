cmake_minimum_required(VERSION 3.31)
project(neural_ants)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


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

# download the RaylibCPP
FetchContent_Declare(
        raylib-cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp
        GIT_TAG v5.5.0
)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0
)

FetchContent_MakeAvailable(yaml-cpp)
FetchContent_MakeAvailable(FMT_CXX)
FetchContent_MakeAvailable(raylib)
FetchContent_MakeAvailable(raylib-cpp)

add_executable(neural_ants texture_cache.hpp main.cpp ant.cpp population.cpp  resources.cpp
        resources.hpp neuron.cpp neural_network.cpp genome.cpp)
target_link_libraries(neural_ants PRIVATE raylib_cpp raylib fmt::fmt yaml-cpp)
