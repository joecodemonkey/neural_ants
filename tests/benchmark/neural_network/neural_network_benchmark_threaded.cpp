#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>

#include "neural_network_benchmark_base.hpp"

class ThreadedNeuralNetworkBenchmark : public NeuralNetworkBenchmarkBase {
 public:
  ThreadedNeuralNetworkBenchmark() = default;
  ThreadedNeuralNetworkBenchmark(const std::string& name) : NeuralNetworkBenchmarkBase(name) {};

  void setup() {
    _network.enable_threads();
  }
};

// Single forward pass benchmark
TEST_CASE_METHOD(ThreadedNeuralNetworkBenchmark,
                 "Threaded Neural Network Benchmark - Single Forward Pass",
                 "[benchmark]") {
  ThreadedNeuralNetworkBenchmark benchmark(
      std::string("Threaded Neural Network Benchmark - Single Forward Pass"));
  benchmark.setup();
  benchmark.reset();
  benchmark.run();
  benchmark.display();
}

// Multiple forward passes benchmark
TEST_CASE_METHOD(ThreadedNeuralNetworkBenchmark,
                 "Threaded Neural Network Benchmark - 100 Forward Passes",
                 "[benchmark]") {
  ThreadedNeuralNetworkBenchmark benchmark(
      std::string("Threaded Neural Network Benchmark - 100 Forward Passes"));
  benchmark.setup();
  std::chrono::nanoseconds duration{0};
  for (size_t i = 0; i < 100; ++i) {
    benchmark.reset();
    benchmark.run();
  }
  benchmark.display();
}