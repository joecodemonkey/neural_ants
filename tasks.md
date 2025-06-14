# Neural Network Benchmarking Tasks

## Overview
Create a parallel benchmarking structure for neural networks similar to the existing neuron benchmarks. The benchmarks will compare threaded vs non-threaded performance of neural networks.

## Files to Create

### 1. Base Neural Network Benchmark Class
**Path**: `tests/benchmark/neural_network/neural_network_benchmark_base.hpp`
- Header file defining the base class for neural network benchmarks
- Should inherit from `BenchmarkBase` in `tests/benchmark/benchmark_base.hpp`
- Define common methods needed to benchmark the network
- Include methods for:
  - Setting up neural network with configurable layers
  - Generating random inputs
  - Resetting the network state
  - Running the benchmark
  - Use the default neural network for counts
  - look at `tests/benchmark/neuron/neuron_benchmark_base.cpp` for hints.

### 2. Base Neural Network Benchmark Implementation
**Path**: `tests/benchmark/neural_network/neural_network_benchmark_base.cpp`
- Implementation of the base neural network benchmark class
- Implement methods for:
  - Generating random inputs
  - Resetting network state
  - Running the benchmark

### 3. Non-Threaded Neural Network Benchmark
**Path**: `tests/benchmark/neural_network/neural_network_benchmark_nonthreaded.cpp`
- Test cases for non-threaded neural network performance
- Include benchmarks for:
  - Single forward pass
  - Multiple forward passes (e.g., 100 passes)
- Should disable threading in the neural network

### 4. Threaded Neural Network Benchmark
**Path**: `tests/benchmark/neural_network/neural_network_benchmark_threaded.cpp`
- Test cases for threaded neural network performance
- Mirror the non-threaded benchmarks but with threading enabled
- Include benchmarks for:
  - Single forward pass
  - Multiple forward passes (e.g., 100 passes)
- Should enable threading in the neural network

## Implementation Details

### Neural Network Configuration
- Use a standard network architecture for consistent benchmarking
- Suggested configuration:
  - Input layer: 200 neurons (matching neuron benchmark)
  - Hidden layer: 100 neurons
  - Output layer: 10 neurons
- Use the same random input generation approach as neuron benchmarks

### Benchmark Cases
1. Single Forward Pass
   - Measure time for one complete forward pass through the network
   - Compare threaded vs non-threaded performance

2. Multiple Forward Passes
   - Run 100 forward passes
   - Measure total duration
   - Compare threaded vs non-threaded performance

### Required Methods
Each benchmark class should implement:
- Constructor with name parameter
- `setup()` method to configure threading
- `reset()` method to reset network state
- `derived_run()` method for the actual benchmark execution

## Notes
- Ensure consistent random input generation across all benchmarks
- Use the same timing mechanism as the neuron benchmarks
- Maintain the same output format for benchmark results
- Consider adding configuration options for network size and layer count 