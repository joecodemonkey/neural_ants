#pragma once
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <neural_network.hpp>
#include <neuron.hpp>
#include <random_generator.hpp>

// Common benchmark setup and utilities
class BenchmarkBase {
 public:
  BenchmarkBase() {};
  BenchmarkBase(const std::string& name) : _name((name)) {};
  virtual auto reset() -> void = 0;
  auto get_duration() -> std::chrono::microseconds;
  auto get_duration_ns() -> std::chrono::nanoseconds;
  auto get_duration_ms() -> std::chrono::milliseconds;
  auto display() -> void;
  auto set_name(const std::string& name) -> void;
  auto run() -> void;

 protected:
  virtual auto derived_run() -> void = 0;
  std::chrono::duration<double> _duration;
  std::string _name;
};