#include "benchmark_base.hpp"

#include <iostream>

auto BenchmarkBase::get_duration() -> std::chrono::microseconds {
  return std::chrono::duration_cast<std::chrono::microseconds>(_duration);
}

auto BenchmarkBase::set_name(const std::string& name) -> void {
  _name = name;
}

auto BenchmarkBase::display() -> void {
  std::cout << _name << ": " << get_duration() << "\n";
}

auto BenchmarkBase::run() -> void {
  auto start = std::chrono::high_resolution_clock::now();
  derived_run();
  auto stop = std::chrono::high_resolution_clock::now();
  _duration += std::chrono::duration<double>(stop - start);
}