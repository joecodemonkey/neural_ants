#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>

#include "neural_network_benchmark_base.hpp"

class NeuronOnlyThreadedBenchmark : public NeuralNetworkBenchmarkBase {
 public:
  NeuronOnlyThreadedBenchmark() = default;
  NeuronOnlyThreadedBenchmark(const std::string& name) : NeuralNetworkBenchmarkBase(name) {};

  void setup() {
    _network.disable_network_threads();
    _network.enable_neuron_threads();
  }
};

// Single forward pass benchmark
TEST_CASE_METHOD(NeuronOnlyThreadedBenchmark,
                 "Neuron-Only Threaded Neural Network Benchmark - Single Forward Pass",
                 "[benchmark]") {
  NeuronOnlyThreadedBenchmark benchmark(
      std::string("Neuron-Only Threaded Neural Network Benchmark - Single Forward Pass"));
  benchmark.setup();
  benchmark.reset();
  benchmark.run();
  benchmark.display();
}

// Multiple forward passes benchmark
TEST_CASE_METHOD(NeuronOnlyThreadedBenchmark,
                 "Neuron-Only Threaded Neural Network Benchmark - 100 Forward Passes",
                 "[benchmark]") {
  NeuronOnlyThreadedBenchmark benchmark(
      std::string("Neuron-Only Threaded Neural Network Benchmark - 100 Forward Passes"));
  benchmark.setup();
  std::chrono::nanoseconds duration{0};
  for (size_t i = 0; i < 100; ++i) {
    benchmark.reset();
    benchmark.run();
  }
  benchmark.display();
}