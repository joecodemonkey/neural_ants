#pragma once
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <memory>
#include <population.hpp>
#include <world.hpp>

#include "../benchmark_base.hpp"

// Base class for population benchmarks
class PopulationBenchmarkBase : public BenchmarkBase {
 public:
  PopulationBenchmarkBase() = default;
  PopulationBenchmarkBase(const std::string& name) : BenchmarkBase(name) {};
  auto reset() -> void override;

 public:
  // Standard time delta for consistent benchmarks
  static constexpr float TIME_DELTA = 0.016f;  // 60 FPS timing

 protected:
  auto derived_run() -> void override;

  // Member variables
  World _world;
  std::unique_ptr<Population> _population;
  size_t _populationSize = 100;
  size_t _iterations = 100;

  // Helper methods
  auto setup_population(size_t size) -> void;
  auto run_population_updates(size_t iterations) -> void;
};