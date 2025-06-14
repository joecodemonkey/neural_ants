#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>

#include "neuron_benchmark_base.hpp"

class NonThreadedNeuronBenchmark : public NeuronBenchmarkBase {
 public:
  NonThreadedNeuronBenchmark() = default;
  NonThreadedNeuronBenchmark(const std::string& name) : NeuronBenchmarkBase(name) {};
  void setup() {
    _neuron.disable_threads();
  }
};

// Reuse the same benchmark cases but with threading disabled
TEST_CASE_METHOD(NonThreadedNeuronBenchmark,
                 "Non-Threaded Neuron Calculation Benchmark - Single Neuron",
                 "[benchmark]") {
  NonThreadedNeuronBenchmark benchmark(
      std::string("Non-Threaded Neuron Calculation Benchmark - Single Neuron"));
  benchmark.setup();
  benchmark.reset();
  benchmark.run();
  benchmark.display();
}

TEST_CASE_METHOD(NonThreadedNeuronBenchmark,
                 "Non-Threaded Multiple Neurons Benchmark - 100 Neuron",
                 "[benchmark]") {
  NonThreadedNeuronBenchmark benchmark(
      std::string("Non-Threaded Multiple Neurons Benchmark - 100 Neuron Duration"));
  benchmark.setup();
  std::chrono::nanoseconds duration{0};
  for (size_t i = 0; i < 100; ++i) {
    benchmark.reset();
    benchmark.run();
  };
  benchmark.display();
}
