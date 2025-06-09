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

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(FMT_CXX)
FetchContent_MakeAvailable(raylib)
FetchContent_MakeAvailable(Catch2)

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

# Create library target
add_library(neural_ants_lib STATIC
    ${C_SOURCES}
    ${CXX_SOURCES}
)

target_link_libraries(neural_ants_lib PRIVATE raylib fmt::fmt)

# Add include directories
target_include_directories(neural_ants_lib PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create main executable
add_executable(neural_ants src/main.cpp)
target_link_libraries(neural_ants PRIVATE neural_ants_lib)
target_include_directories(neural_ants PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/fmt_cxx-src/include
)

# Test configuration
enable_testing()

# Find all test files recursively
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

# Create test executable
add_executable(neural_ants_tests ${TEST_SOURCES})

# Link test executable with Catch2
target_link_libraries(neural_ants_tests PRIVATE
    Catch2::Catch2WithMain
    neural_ants_lib
)

# Include directories for tests
target_include_directories(neural_ants_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/catch2-src/src
)

# Register tests with CTest
include(CTest)
include(Catch)
catch_discover_tests(neural_ants_tests)
