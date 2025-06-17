cmake_minimum_required(VERSION 3.10)
project(neural_ants LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable parallel algorithms and OpenMP
find_package(OpenMP REQUIRED)
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/TBB")
find_package(TBB REQUIRED)

# Enable parallel algorithms
set(PARALLEL_FLAGS "-D_GLIBCXX_PARALLEL -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

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

FetchContent_Declare(
        ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.91.9b
)

FetchContent_Declare(
    rlImGui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui    
    GIT_TAG main
)

FetchContent_MakeAvailable(FMT_CXX)
FetchContent_MakeAvailable(raylib)
FetchContent_MakeAvailable(Catch2)
FetchContent_MakeAvailable(ImGui)
FetchContent_MakeAvailable(rlImGui)

# Grab the headers and sources for IMGUI
file(GLOB IMGUI_SOURCES "${imgui_SOURCE_DIR}/*.cpp")
file(GLOB IMGUI_HEADERS "${imgui_SOURCE_DIR}/*.h")

# Create ImGui library
add_library(imgui_lib STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES} )
target_include_directories(imgui_lib PUBLIC ${imgui_SOURCE_DIR})

file(GLOB RLIMGUI_SOURCES "${rlimgui_SOURCE_DIR}/*.cpp")
file(GLOB RLIMGUI_HEADERS "${rlimgui_SOURCE_DIR}/*.cpp")

# Create rlImGui library
add_library(rlimgui_lib STATIC ${RLIMGUI_HEADERS} ${RLIMGUI_SOURCES})

target_include_directories(rlimgui_lib PUBLIC 
    ${rlimgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
)

target_link_libraries(rlimgui_lib PUBLIC imgui_lib raylib)


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
    src/ui.cpp
)

# Create library target
add_library(neural_ants_lib STATIC
    ${C_SOURCES}
    ${CXX_SOURCES}
)
target_compile_options(neural_ants_lib PRIVATE ${PARALLEL_FLAGS})
target_link_libraries(neural_ants_lib PRIVATE raylib imgui_lib rlimgui_lib fmt::fmt TBB::tbb OpenMP::OpenMP_CXX pthread)

# Add include directories
target_include_directories(neural_ants_lib PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR}
    ${rlimgui_SOURCE_DIR}
)

# Create main executable
add_executable(neural_ants src/main.cpp)
target_compile_options(neural_ants PRIVATE ${PARALLEL_FLAGS})
target_link_libraries(neural_ants PRIVATE neural_ants_lib raylib imgui_lib rlimgui_lib fmt::fmt TBB::tbb OpenMP::OpenMP_CXX pthread)
target_include_directories(neural_ants PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/fmt_cxx-src/include
)

# Test configuration
enable_testing()

# Find all unit test files recursively
file(GLOB_RECURSE UNIT_TEST_SOURCES "tests/unit/*.cpp")

# Create unit test executable
add_executable(neural_ants_unit_tests ${UNIT_TEST_SOURCES})

# Link unit test executable with Catch2
target_link_libraries(neural_ants_unit_tests PRIVATE
    Catch2::Catch2WithMain
    neural_ants_lib
    raylib
    fmt::fmt
    TBB::tbb
    OpenMP::OpenMP_CXX
    pthread
)

# Include directories for unit tests
target_include_directories(neural_ants_unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/catch2-src/src
)

# Register unit tests with CTest
include(CTest)
include(Catch)
catch_discover_tests(neural_ants_unit_tests)

# Benchmark configuration
file(GLOB_RECURSE BENCHMARK_SOURCES "tests/benchmark/*.cpp")

# Create benchmark executable
add_executable(neural_ants_benchmarks ${BENCHMARK_SOURCES})

# Set release mode and optimization flags for benchmarks
set_target_properties(neural_ants_benchmarks PROPERTIES
    CMAKE_BUILD_TYPE Release
)

# Set high optimization flags for benchmarks
target_compile_options(neural_ants_benchmarks PRIVATE
    -O3
    -march=native
    -ffast-math
    ${PARALLEL_FLAGS}
)

# Link benchmark executable with Catch2
target_link_libraries(neural_ants_benchmarks PRIVATE
    Catch2::Catch2WithMain
    neural_ants_lib
    raylib
    fmt::fmt
    TBB::tbb
    OpenMP::OpenMP_CXX
    pthread
)

# Include directories for benchmarks
target_include_directories(neural_ants_benchmarks PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/catch2-src/src
)

# Register benchmarks with CTest
catch_discover_tests(neural_ants_benchmarks)
