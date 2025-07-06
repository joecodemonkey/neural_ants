#include "benchmark_base.hpp"

#include <chrono>
#include <iostream>

auto BenchmarkBase::get_duration() -> std::chrono::microseconds {
  return std::chrono::duration_cast<std::chrono::microseconds>(_duration);
}

auto BenchmarkBase::get_duration_ns() -> std::chrono::nanoseconds {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(_duration);
}

auto BenchmarkBase::get_duration_ms() -> std::chrono::milliseconds {
  return std::chrono::duration_cast<std::chrono::milliseconds>(_duration);
}

auto BenchmarkBase::set_name(const std::string& name) -> void {
  _name = name;
}

auto BenchmarkBase::display() -> void {
  auto duration = get_duration();
  if (duration.count() > 1000) {
    std::cout << _name << ": " << get_duration_ms() << "\n";
  } else {
    std::cout << _name << ": " << get_duration() << "\n";
  }
}

auto BenchmarkBase::run() -> void {
  auto start = std::chrono::high_resolution_clock::now();
  derived_run();
  auto stop = std::chrono::high_resolution_clock::now();
  _duration = std::chrono::duration<double>(stop - start);
}