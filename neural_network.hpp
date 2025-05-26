#pragma once
#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "neuron.hpp"

// A fully connected neural network
class NeuralNetwork {
 public:
  typedef std::vector<Neuron> Layer;
  typedef std::vector<Neuron::Value> ValueVector;

  NeuralNetwork() = default;

  auto set_input(const ValueVector& input) -> void;
  auto set_input(size_t idx, Neuron::Value value) -> void;

  auto get_inputs() const -> const ValueVector&;

  auto get_input(size_t idx) const -> Neuron::Value;

  auto get_outputs() -> const ValueVector&;
  auto get_output(size_t idx) -> Neuron::Value;

  auto set_input_layer_count(size_t count) -> void;
  auto get_input_layer_count() const -> size_t;

  auto set_output_layer_count(size_t count) -> void;
  auto get_output_layer_count() const -> size_t;

  auto get_layer(size_t idx) -> Layer&;
  auto get_layer(size_t idx) const -> const Layer&;

  auto set_hidden_layer_count(size_t count) -> void;
  auto get_hidden_layer_count() const -> size_t;

  auto get_output_layer() -> Layer&;
  auto get_output_layer() const -> const Layer&;

  auto randomize() -> void;

 protected:
  std::vector<Layer> _layers;
  Layer _output_layer;
  ValueVector _inputs;
  ValueVector _outputs;

  bool _ready;
};
