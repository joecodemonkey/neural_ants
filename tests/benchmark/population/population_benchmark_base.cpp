#include "population_benchmark_base.hpp"

#include <iostream>

auto PopulationBenchmarkBase::reset() -> void {
  _world = World(_textureCache);
  _population = std::make_unique<Population>(_world);
  setup_population(_populationSize);
}

auto PopulationBenchmarkBase::derived_run() -> void {
  run_population_updates(_iterations);
}

auto PopulationBenchmarkBase::setup_population(size_t size) -> void {
  _population->set_size(static_cast<int>(size));
  // Initialize population by running one update to create ants
  _population->update(0.001f);
}

auto PopulationBenchmarkBase::run_population_updates(size_t iterations) -> void {
  for (size_t i = 0; i < iterations; ++i) {
    _population->update(TIME_DELTA);
  }
}