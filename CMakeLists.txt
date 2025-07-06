cmake_minimum_required(VERSION 3.10)
project(neural_ants LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable parallel algorithms

# Enable threading
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Build configuration - Release by default, Debug if NEURAL_ANTS_DEBUG is set
if(DEFINED ENV{NEURAL_ANTS_DEBUG})
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
    message(STATUS "Debug build enabled via NEURAL_ANTS_DEBUG environment variable")
else()
    set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -DNDEBUG -Wno-stringop-overflow")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native -DNDEBUG -Wno-stringop-overflow")
    message(STATUS "Release build with -O3 optimization")
endif()

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
        ImPlot
        GIT_REPOSITORY https://github.com/epezent/implot
        GIT_TAG master
)

FetchContent_Declare(
    rlImGui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui    
    GIT_TAG main
)

FetchContent_Declare(
    oneTBB
    GIT_REPOSITORY https://github.com/uxlfoundation/oneTBB.git
    GIT_TAG v2021.13.0
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

# Disable oneTBB tests and examples to avoid build issues
set(TBB_TEST OFF CACHE BOOL "" FORCE)
set(TBB_EXAMPLES OFF CACHE BOOL "" FORCE)


FetchContent_MakeAvailable(FMT_CXX)
FetchContent_MakeAvailable(raylib)
FetchContent_MakeAvailable(Catch2)
FetchContent_MakeAvailable(ImGui)
FetchContent_MakeAvailable(ImPlot)
FetchContent_MakeAvailable(rlImGui)
FetchContent_MakeAvailable(oneTBB)
FetchContent_MakeAvailable(nlohmann_json)

# Grab the headers and sources for IMGUI
file(GLOB IMGUI_SOURCES "${imgui_SOURCE_DIR}/*.cpp")
file(GLOB IMGUI_HEADERS "${imgui_SOURCE_DIR}/*.h")

# Create ImGui library
add_library(imgui_lib STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES} )
target_include_directories(imgui_lib PUBLIC ${imgui_SOURCE_DIR})

# Grab the headers and sources for IMPLOT
file(GLOB IMPLOT_SOURCES "${implot_SOURCE_DIR}/*.cpp")
file(GLOB IMPLOT_HEADERS "${implot_SOURCE_DIR}/*.h")

# Create ImPlot library
add_library(implot_lib STATIC ${IMPLOT_HEADERS} ${IMPLOT_SOURCES})
target_include_directories(implot_lib PUBLIC ${implot_SOURCE_DIR})
target_link_libraries(implot_lib PUBLIC imgui_lib)

file(GLOB RLIMGUI_SOURCES "${rlimgui_SOURCE_DIR}/*.cpp")
file(GLOB RLIMGUI_HEADERS "${rlimgui_SOURCE_DIR}/*.cpp")

# Create rlImGui library
add_library(rlimgui_lib STATIC ${RLIMGUI_HEADERS} ${RLIMGUI_SOURCES})

target_include_directories(rlimgui_lib PUBLIC 
    ${rlimgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
)

target_link_libraries(rlimgui_lib PUBLIC imgui_lib implot_lib raylib)


# Separate C and C++ sources
set(C_SOURCES
    src/raylibdrawex.c
    src/raylibmathex.c
)

set(CXX_SOURCES
    src/util/file.cpp
    src/util/serialization.cpp
    src/texture_cache.cpp
    src/main.cpp
    src/game.cpp 
    src/input.cpp
    src/world.cpp
    src/ant.cpp
    src/ant_renderer.cpp
    src/population.cpp
    src/resources.cpp
    src/neuron.cpp
    src/neural_network.cpp
    src/genome.cpp
    src/surroundings.cpp
    src/food.cpp
    src/brain.cpp
    src/ui/renderer.cpp
    src/ui/buttons.cpp
    src/ui/menu/settings.cpp
    src/ui/menu/save_load.cpp
    src/ui/menu/fitness_display.cpp
    src/ui/state.cpp
)

# Create library target
add_library(neural_ants_lib STATIC
    ${C_SOURCES}
    ${CXX_SOURCES}
)
target_link_libraries(neural_ants_lib PRIVATE raylib imgui_lib implot_lib rlimgui_lib fmt::fmt TBB::tbb nlohmann_json::nlohmann_json pthread stdc++fs)

# Add include directories
target_include_directories(neural_ants_lib PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR}
    ${rlimgui_SOURCE_DIR}
)

# Create main executable
add_executable(neural_ants src/main.cpp)
target_link_libraries(neural_ants PRIVATE neural_ants_lib raylib imgui_lib implot_lib rlimgui_lib fmt::fmt TBB::tbb nlohmann_json::nlohmann_json pthread stdc++fs)
target_include_directories(neural_ants PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/fmt_cxx-src/include
    ${implot_SOURCE_DIR}
)

# Test configuration
enable_testing()

# Find all unit test files recursively
file(GLOB_RECURSE UNIT_TEST_SOURCES "tests/unit/*.cpp")

# Create unit test executable
add_executable(neural_ants_unit_tests ${UNIT_TEST_SOURCES} ${BENCHMARK_HELPER_SOURCES})

# Link unit test executable with Catch2
target_link_libraries(neural_ants_unit_tests PRIVATE
    Catch2::Catch2WithMain
    neural_ants_lib
    raylib
    imgui_lib
    implot_lib
    fmt::fmt
    TBB::tbb
    nlohmann_json::nlohmann_json
    pthread
    stdc++fs
)

# Include directories for unit tests
target_include_directories(neural_ants_unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/catch2-src/src
    ${implot_SOURCE_DIR}
)

# Register unit tests with CTest
include(CTest)
include(Catch)
catch_discover_tests(neural_ants_unit_tests)

# Benchmark configuration
file(GLOB_RECURSE BENCHMARK_SOURCES "tests/benchmark/*.cpp")
file(GLOB_RECURSE BENCHMARK_HELPER_SOURCES "src/tests/helpers/*.cpp")

# Create benchmark executable
add_executable(neural_ants_benchmarks ${BENCHMARK_SOURCES} ${BENCHMARK_HELPER_SOURCES})

# Add fast-math for benchmarks (optimization already handled globally)
target_compile_options(neural_ants_benchmarks PRIVATE
    -ffast-math
)

# Link benchmark executable with Catch2
target_link_libraries(neural_ants_benchmarks PRIVATE
    Catch2::Catch2WithMain
    neural_ants_lib
    raylib
    imgui_lib
    implot_lib
    fmt::fmt
    TBB::tbb
    nlohmann_json::nlohmann_json
    pthread
    stdc++fs
)

# Include directories for benchmarks
target_include_directories(neural_ants_benchmarks PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/raylib-src/src
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/catch2-src/src
    ${implot_SOURCE_DIR}
)

# Register benchmarks with CTest
catch_discover_tests(neural_ants_benchmarks)
