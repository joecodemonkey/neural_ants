cmake_minimum_required(VERSION 3.31)
project(neural_ants)

include(FetchContent)

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

# Make Raylib available
FetchContent_MakeAvailable(raylib)
FetchContent_MakeAvailable(raylib-cpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(neural_ants main.cpp)
target_link_libraries(neural_ants PRIVATE raylib_cpp raylib)
