#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>

#include "neuron_benchmark_base.hpp"

class ThreadedNeuronBenchmark : public NeuronBenchmarkBase {
 public:
  void setup() {
    _neuron.enable_threads();
  }
};

// Reuse the same benchmark cases but with threading disabled
TEST_CASE_METHOD(ThreadedNeuronBenchmark,
                 "Threaded Neuron Calculation Benchmark - Single Neuron",
                 "[benchmark]") {
  ThreadedNeuronBenchmark benchmark(
      std::string("Threaded Neuron Calculation Benchmark - Single Neuron"));
  benchmark.setup();
  benchmark.reset();
  benchmark.run();
  benchmark.display();
}

TEST_CASE_METHOD(ThreadedNeuronBenchmark,
                 "Threaded Multiple Neurons Benchmark - 100 Neuron",
                 "[benchmark]") {
  ThreadedNeuronBenchmark benchmark(
      std::string("Threaded Multiple Neurons Benchmark - 100 Neuron"));
  benchmark.setup();
  std::chrono::nanoseconds duration{0};
  for (size_t i = 0; i < 100; ++i) {
    benchmark.reset();
    benchmark.run();
  };
  benchmark.display();
}