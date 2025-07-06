#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class BenchmarkReporter {
 public:
  BenchmarkReporter() = default;
  BenchmarkReporter(const std::string& title, const std::string& output_file);

  auto set_title(const std::string& title) -> void;
  auto set_data(const std::vector<double>& data) -> void;
  auto generate_report() -> void;
  auto write_to_file() -> void;

 private:
  std::string _title;
  std::string _output_file;
  std::vector<double> _raw_data;
  std::string _report_content;
  double _mean;
  double _std_dev;
  double _min;
  double _max;

  auto calculate_statistics() -> void;
  auto format_markdown_table() -> std::string;
  auto format_raw_data_table() -> std::string;
  auto get_system_info() -> std::string;
  auto get_timestamp() -> std::string;
  auto get_compiler_info() -> std::string;

  // Smart unit selection methods
  auto determine_best_unit() -> std::string;
  auto convert_to_best_unit(double value_ns) -> double;
  auto get_unit_conversion_factor() -> double;
};

class StatisticalBenchmarkRunner {
 public:
  static constexpr size_t NUM_ITERATIONS = 30;

  template <typename BenchmarkType>
  static auto run_statistical_benchmark(BenchmarkType& benchmark) -> std::vector<double> {
    std::vector<double> results;
    results.reserve(NUM_ITERATIONS);

    for (size_t i = 0; i < NUM_ITERATIONS; ++i) {
      benchmark.reset();
      benchmark.run();
      results.push_back(benchmark.get_duration().count());
    }

    return results;
  }

  static auto calculate_mean(const std::vector<double>& data) -> double;
  static auto calculate_std_dev(const std::vector<double>& data, double mean) -> double;
  static auto get_min(const std::vector<double>& data) -> double;
  static auto get_max(const std::vector<double>& data) -> double;
};