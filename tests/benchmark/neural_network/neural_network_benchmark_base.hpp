#pragma once
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <neural_network.hpp>
#include <random_generator.hpp>

#include "../benchmark_base.hpp"

// Base class for neural network benchmarks
class NeuralNetworkBenchmarkBase : public BenchmarkBase {
 public:
  NeuralNetworkBenchmarkBase() = default;
  NeuralNetworkBenchmarkBase(const std::string& name) : BenchmarkBase(name) {};
  auto reset() -> void override;

 protected:
  auto derived_run() -> void override;

  // Network configuration constants
  static constexpr size_t INPUT_LAYER_SIZE = 200;  // Matching neuron benchmark
  static constexpr size_t HIDDEN_LAYER_SIZE = 100;
  static constexpr size_t OUTPUT_LAYER_SIZE = 2;

  // Member variables
  NeuralNetwork _network;
  NeuralNetwork::ValueVector _inputs;
  RandomGenerator _randomGenerator;

  // Helper methods
  auto generate_random_inputs() -> void;
  auto setup_network() -> void;
};