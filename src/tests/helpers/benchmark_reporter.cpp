#include "tests/helpers/benchmark_reporter.hpp"

#include <sys/utsname.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

BenchmarkReporter::BenchmarkReporter(const std::string& title, const std::string& output_file)
    : _title(title), _output_file(output_file) {}

auto BenchmarkReporter::set_title(const std::string& title) -> void {
  _title = title;
}

auto BenchmarkReporter::set_data(const std::vector<double>& data) -> void {
  _raw_data = data;
}

auto BenchmarkReporter::generate_report() -> void {
  calculate_statistics();

  std::stringstream report;
  report << "# " << _title << "\n\n";

  // Test Environment section
  report << "## Test Environment\n";
  report << "- Date: " << get_timestamp() << "\n";
  report << "- System: " << get_system_info() << "\n";
  report << "- Compiler: " << get_compiler_info() << "\n\n";

  // Statistical Summary
  report << "## Statistical Summary\n\n";
  report << format_markdown_table();

  // Raw Data
  report << "\n## Raw Data\n\n";
  report << format_raw_data_table();

  _report_content = report.str();
}

auto BenchmarkReporter::write_to_file() -> void {
  std::ofstream file(_output_file);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << _output_file << " for writing\n";
    return;
  }

  file << _report_content;
  file.close();

  std::cout << "Benchmark report written to: " << _output_file << std::endl;
}

auto BenchmarkReporter::calculate_statistics() -> void {
  if (_raw_data.empty()) {
    _mean = _std_dev = _min = _max = 0.0;
    return;
  }

  _mean = StatisticalBenchmarkRunner::calculate_mean(_raw_data);
  _std_dev = StatisticalBenchmarkRunner::calculate_std_dev(_raw_data, _mean);
  _min = StatisticalBenchmarkRunner::get_min(_raw_data);
  _max = StatisticalBenchmarkRunner::get_max(_raw_data);
}

auto BenchmarkReporter::format_markdown_table() -> std::string {
  std::string unit = determine_best_unit();

  std::stringstream table;
  table << "| Metric | Value |\n";
  table << "|--------|-------|\n";
  table << "| Iterations | " << _raw_data.size() << " |\n";
  table << "| Mean (" << unit << ") | " << std::fixed << std::setprecision(2)
        << convert_to_best_unit(_mean) << " |\n";
  table << "| Std Dev (" << unit << ") | " << std::fixed << std::setprecision(2)
        << convert_to_best_unit(_std_dev) << " |\n";
  table << "| Min (" << unit << ") | " << std::fixed << std::setprecision(2)
        << convert_to_best_unit(_min) << " |\n";
  table << "| Max (" << unit << ") | " << std::fixed << std::setprecision(2)
        << convert_to_best_unit(_max) << " |\n";

  return table.str();
}

auto BenchmarkReporter::format_raw_data_table() -> std::string {
  std::string unit = determine_best_unit();

  std::stringstream table;
  table << "| Run | Duration (" << unit << ") |\n";
  table << "|-----|---------------|\n";

  for (size_t i = 0; i < _raw_data.size(); ++i) {
    table << "| " << (i + 1) << " | " << std::fixed << std::setprecision(2)
          << convert_to_best_unit(_raw_data[i]) << " |\n";
  }

  return table.str();
}

auto BenchmarkReporter::get_system_info() -> std::string {
  struct utsname system_info;
  if (uname(&system_info) != 0) {
    return "Unknown system";
  }

  std::stringstream info;
  info << system_info.sysname << " " << system_info.release << " " << system_info.machine;

  return info.str();
}

auto BenchmarkReporter::get_timestamp() -> std::string {
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream timestamp;
  timestamp << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");

  return timestamp.str();
}

auto BenchmarkReporter::get_compiler_info() -> std::string {
  std::stringstream info;

#ifdef __GNUC__
  info << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
#elif defined(__clang__)
  info << "Clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__;
#elif defined(_MSC_VER)
  info << "MSVC " << _MSC_VER;
#else
  info << "Unknown compiler";
#endif

  return info.str();
}

// StatisticalBenchmarkRunner implementation
auto StatisticalBenchmarkRunner::calculate_mean(const std::vector<double>& data) -> double {
  if (data.empty())
    return 0.0;

  double sum = std::accumulate(data.begin(), data.end(), 0.0);
  return sum / data.size();
}

auto StatisticalBenchmarkRunner::calculate_std_dev(const std::vector<double>& data, double mean)
    -> double {
  if (data.empty())
    return 0.0;

  double variance = 0.0;
  for (const auto& value : data) {
    variance += (value - mean) * (value - mean);
  }
  variance /= data.size();

  return std::sqrt(variance);
}

auto StatisticalBenchmarkRunner::get_min(const std::vector<double>& data) -> double {
  if (data.empty())
    return 0.0;
  return *std::min_element(data.begin(), data.end());
}

auto StatisticalBenchmarkRunner::get_max(const std::vector<double>& data) -> double {
  if (data.empty())
    return 0.0;
  return *std::max_element(data.begin(), data.end());
}

// BenchmarkReporter smart unit selection methods
auto BenchmarkReporter::determine_best_unit() -> std::string {
  if (_mean >= 1000000.0) {  // >= 1 ms
    return "ms";
  } else if (_mean >= 1000.0) {  // >= 1 μs
    return "μs";
  } else {
    return "ns";
  }
}

auto BenchmarkReporter::convert_to_best_unit(double value_ns) -> double {
  std::string unit = determine_best_unit();

  if (unit == "ms") {
    return value_ns / 1000000.0;
  } else if (unit == "μs") {
    return value_ns / 1000.0;
  } else {
    return value_ns;  // ns
  }
}

auto BenchmarkReporter::get_unit_conversion_factor() -> double {
  std::string unit = determine_best_unit();

  if (unit == "ms") {
    return 1000000.0;
  } else if (unit == "μs") {
    return 1000.0;
  } else {
    return 1.0;  // ns
  }
}