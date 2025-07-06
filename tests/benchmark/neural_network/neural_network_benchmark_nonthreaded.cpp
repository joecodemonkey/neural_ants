#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>
#include <vector>

#include "neural_network_benchmark_base.hpp"
#include "tests/helpers/benchmark_reporter.hpp"

class NonThreadedNeuralNetworkBenchmark : public NeuralNetworkBenchmarkBase {
 public:
  NonThreadedNeuralNetworkBenchmark() = default;
  NonThreadedNeuralNetworkBenchmark(const std::string& name) : NeuralNetworkBenchmarkBase(name) {};
};

TEST_CASE("Statistical Non-Threaded Neural Network Benchmarks", "[benchmark]") {
  std::cout << "Running statistical neural network benchmarks with 30 iterations each...\n";

  // Test Case 1: Single Forward Pass
  {
    const std::string test_name = "Non-Threaded Neural Network Benchmark - Single Forward Pass";
    std::cout << "Running: " << test_name << "\n";

    std::vector<double> single_pass_data;
    single_pass_data.reserve(StatisticalBenchmarkRunner::NUM_ITERATIONS);

    for (size_t i = 0; i < StatisticalBenchmarkRunner::NUM_ITERATIONS; ++i) {
      NonThreadedNeuralNetworkBenchmark benchmark(test_name);
      benchmark.reset();
      benchmark.run();
      // Use nanoseconds for better precision
      single_pass_data.push_back(static_cast<double>(benchmark.get_duration_ns().count()));
    }

    BenchmarkReporter single_reporter(test_name, "single_neural_network_benchmark.md");
    single_reporter.set_data(single_pass_data);
    single_reporter.generate_report();
    single_reporter.write_to_file();

    std::cout << "Completed: " << test_name
              << " - Report saved to single_neural_network_benchmark.md\n";
  }

  // Test Case 2: 100 Forward Passes Sequential
  {
    const std::string test_name = "Non-Threaded Neural Network Benchmark - 100 Forward Passes";
    std::cout << "Running: " << test_name << "\n";

    std::vector<double> multi_pass_data;
    multi_pass_data.reserve(StatisticalBenchmarkRunner::NUM_ITERATIONS);

    for (size_t i = 0; i < StatisticalBenchmarkRunner::NUM_ITERATIONS; ++i) {
      NonThreadedNeuralNetworkBenchmark benchmark(test_name);

      // Run 100 forward passes sequentially and measure total time
      std::chrono::nanoseconds total_duration{0};
      for (size_t j = 0; j < 100; ++j) {
        benchmark.reset();
        benchmark.run();
        total_duration += benchmark.get_duration_ns();
      }

      multi_pass_data.push_back(
          static_cast<double>(total_duration.count()));  // Already in nanoseconds
    }

    BenchmarkReporter multi_reporter(test_name, "multiple_neural_network_benchmark.md");
    multi_reporter.set_data(multi_pass_data);
    multi_reporter.generate_report();
    multi_reporter.write_to_file();

    std::cout << "Completed: " << test_name
              << " - Report saved to multiple_neural_network_benchmark.md\n";
  }

  std::cout << "All statistical neural network benchmarks completed!\n";
}