#pragma once
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <neural_network.hpp>
#include <neuron.hpp>
#include <random_generator.hpp>

#include "../benchmark_base.hpp"

// Common benchmark setup and utilities
class NeuronBenchmarkBase : public BenchmarkBase {
 public:
  NeuronBenchmarkBase() = default;
  NeuronBenchmarkBase(const std::string& name) : BenchmarkBase(name) {};
  auto reset() -> void override;

 protected:
  auto derived_run() -> void override;
  static constexpr size_t NUM_INPUTS = 200;

  NeuralNetwork::ValueVector _inputs;
  RandomGenerator _randomGenerator;
  Neuron _neuron;

  auto generate_random_inputs() -> void;
  auto setup_neuron(Neuron& neuron);
};