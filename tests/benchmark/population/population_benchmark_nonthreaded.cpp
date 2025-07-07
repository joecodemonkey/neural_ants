#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>
#include <vector>

#include "population_benchmark_base.hpp"
#include "tests/helpers/benchmark_reporter.hpp"

// Simplified benchmark implementation
class PopulationUpdateBenchmark : public PopulationBenchmarkBase {
 public:
  PopulationUpdateBenchmark(size_t populationSize, size_t iterations = 100)
      : PopulationBenchmarkBase("Population Update") {
    _populationSize = populationSize;
    _iterations = iterations;
  }
};

// Helper function to run a benchmark and generate report
auto run_population_benchmark(size_t populationSize, const std::string& filename, 
                              size_t iterations = 100) -> void {
  const std::string test_name = "Non-Threaded Population Benchmark - " + std::to_string(populationSize) + " ants";
  std::cout << "Running: " << test_name << "\n";

  std::vector<double> data;
  data.reserve(StatisticalBenchmarkRunner::NUM_ITERATIONS);

  for (size_t i = 0; i < StatisticalBenchmarkRunner::NUM_ITERATIONS; ++i) {
    PopulationUpdateBenchmark benchmark(populationSize, iterations);
    benchmark.reset();
    benchmark.run();
    data.push_back(static_cast<double>(benchmark.get_duration_ns().count()));
  }

  BenchmarkReporter reporter(test_name, filename);
  reporter.set_data(data);
  reporter.generate_report();
  reporter.write_to_file();

  std::cout << "Completed: " << test_name << " - Report saved to " << filename << "\n";
}

TEST_CASE("Statistical Non-Threaded Population Benchmarks", "[population][benchmark]") {
  std::cout << "Running statistical population benchmarks with 30 iterations each...\n";

  // Population size benchmarks
  run_population_benchmark(50, "population_50_benchmark.md");
  run_population_benchmark(100, "population_100_benchmark.md");
  run_population_benchmark(200, "population_200_benchmark.md");

  // Single ant performance (high iteration count)
  run_population_benchmark(1, "single_ant_benchmark.md", 1000);

  std::cout << "All statistical population benchmarks completed!\n";
}