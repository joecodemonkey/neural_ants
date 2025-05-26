#pragma once
#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "neural_network.hpp"

class Genome {
 public:
  // Default constructor
  Genome();

  // Copy constructor
  Genome(const Genome& other);

  // Move constructor
  Genome(Genome&& other) noexcept;

  // Copy assignment
  auto operator=(const Genome& other) -> Genome&;

  // Move assignment
  auto operator=(Genome&& other) noexcept -> Genome&;

  auto breed_with(const Genome& other, double mutationRate = 0.1) -> Genome;

  auto get_network() -> NeuralNetwork&;

  auto mutate(double mutationRate = 0.1) -> void;
  auto randomize() -> void;

  auto set_layer_count(size_t count) -> void;
  auto get_layer_count() const -> size_t;

  auto set_hidden_layer_neuron_count(size_t count) -> void;
  auto get_hidden_layer_neuron_count() const -> size_t;

  auto set_input_count(size_t inputCount) -> void;
  auto get_input_count() const -> size_t;

  auto set_output_count(size_t outputCount) -> void;
  auto get_output_count() const -> size_t;

  auto validate_compatible(const Genome& other) const -> void;

 protected:
  static auto breed_layer(const NeuralNetwork::Layer& parent1,
                          const NeuralNetwork::Layer& parent2,
                          NeuralNetwork::Layer& child) -> void;

  auto create_network() -> void;

  NeuralNetwork _network;
  bool _ready = false;

  // Configuration settings
  size_t _inputCount = 8;
  size_t _outputCount = 3;
  size_t _layerCount = 1;
  size_t _neuronCount = 5;
};
