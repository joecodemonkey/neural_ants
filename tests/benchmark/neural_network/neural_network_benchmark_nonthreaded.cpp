#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>

#include "neural_network_benchmark_base.hpp"

class NonThreadedNeuralNetworkBenchmark : public NeuralNetworkBenchmarkBase {
 public:
  NonThreadedNeuralNetworkBenchmark() = default;
  NonThreadedNeuralNetworkBenchmark(const std::string& name) : NeuralNetworkBenchmarkBase(name) {};
};

// Single forward pass benchmark
TEST_CASE_METHOD(NonThreadedNeuralNetworkBenchmark,
                 "Non-Threaded Neural Network Benchmark - Single Forward Pass",
                 "[benchmark]") {
  NonThreadedNeuralNetworkBenchmark benchmark(
      std::string("Non-Threaded Neural Network Benchmark - Single Forward Pass"));
  benchmark.reset();
  benchmark.run();
  benchmark.display();
}

// Multiple forward passes benchmark
TEST_CASE_METHOD(NonThreadedNeuralNetworkBenchmark,
                 "Non-Threaded Neural Network Benchmark - 100 Forward Passes",
                 "[benchmark]") {
  NonThreadedNeuralNetworkBenchmark benchmark(
      std::string("Non-Threaded Neural Network Benchmark - 100 Forward Passes"));
  std::chrono::nanoseconds duration{0};
  for (size_t i = 0; i < 100; ++i) {
    benchmark.reset();
    benchmark.run();
  }
  benchmark.display();
}