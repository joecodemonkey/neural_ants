#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>
#include <vector>

#include "neuron_benchmark_base.hpp"
#include "tests/helpers/benchmark_reporter.hpp"

class NonThreadedNeuronBenchmark : public NeuronBenchmarkBase {
 public:
  NonThreadedNeuronBenchmark() = default;
  NonThreadedNeuronBenchmark(const std::string& name) : NeuronBenchmarkBase(name) {};
};

TEST_CASE("Statistical Non-Threaded Neuron Benchmarks", "[benchmark]") {
  std::cout << "Running statistical neuron benchmarks with 30 iterations each...\n";

  // Test Case 1: Single Neuron
  {
    const std::string test_name = "Non-Threaded Neuron Calculation Benchmark - Single Neuron";
    std::cout << "Running: " << test_name << "\n";

    std::vector<double> single_neuron_data;
    single_neuron_data.reserve(StatisticalBenchmarkRunner::NUM_ITERATIONS);

    for (size_t i = 0; i < StatisticalBenchmarkRunner::NUM_ITERATIONS; ++i) {
      NonThreadedNeuronBenchmark benchmark(test_name);
      benchmark.reset();
      benchmark.run();
      // Use nanoseconds for better precision
      single_neuron_data.push_back(static_cast<double>(benchmark.get_duration_ns().count()));
    }

    BenchmarkReporter single_reporter(test_name, "single_neuron_benchmark.md");
    single_reporter.set_data(single_neuron_data);
    single_reporter.generate_report();
    single_reporter.write_to_file();

    std::cout << "Completed: " << test_name << " - Report saved to single_neuron_benchmark.md\n";
  }

  // Test Case 2: 100 Neurons Sequential
  {
    const std::string test_name = "Non-Threaded Multiple Neurons Benchmark - 100 Neuron";
    std::cout << "Running: " << test_name << "\n";

    std::vector<double> multi_neuron_data;
    multi_neuron_data.reserve(StatisticalBenchmarkRunner::NUM_ITERATIONS);

    for (size_t i = 0; i < StatisticalBenchmarkRunner::NUM_ITERATIONS; ++i) {
      NonThreadedNeuronBenchmark benchmark(test_name);

      // Run 100 neurons sequentially and measure total time
      std::chrono::nanoseconds total_duration{0};
      for (size_t j = 0; j < 100; ++j) {
        benchmark.reset();
        benchmark.run();
        total_duration += benchmark.get_duration_ns();
      }

      multi_neuron_data.push_back(
          static_cast<double>(total_duration.count()));  // Already in nanoseconds
    }

    BenchmarkReporter multi_reporter(test_name, "multiple_neurons_benchmark.md");
    multi_reporter.set_data(multi_neuron_data);
    multi_reporter.generate_report();
    multi_reporter.write_to_file();

    std::cout << "Completed: " << test_name << " - Report saved to multiple_neurons_benchmark.md\n";
  }

  std::cout << "All statistical benchmarks completed!\n";
}
