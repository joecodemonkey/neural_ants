#pragma once
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <neural_network.hpp>
#include <neuron.hpp>
#include <random_generator.hpp>

// Common benchmark setup and utilities
class NeuronBenchmarkBase {
 public:
  NeuronBenchmarkBase() = default;
  NeuronBenchmarkBase(const std::string& name) : _name((name)) {};
  auto reset() -> void;
  auto run() -> void;
  auto get() -> Neuron&;
  auto get_duration() -> std::chrono::microseconds;
  auto display() -> void;
  auto set_name(const std::string& name) -> void;

 protected:
  static constexpr size_t NUM_INPUTS = 200;
  static constexpr size_t NUM_ITERATIONS = 1000;

  std::chrono::duration<double> _duration;

  NeuralNetwork::ValueVector _inputs;
  RandomGenerator _randomGenerator;
  Neuron _neuron;
  std::string _name;

  auto generate_random_inputs() -> void;
  auto setup_neuron(Neuron& neuron);
};