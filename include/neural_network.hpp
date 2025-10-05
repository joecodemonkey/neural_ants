#pragma once
#include <nlohmann/json.hpp>
#include <vector>

#include "neuron.hpp"
#include "random_generator.hpp"

// A fully connected neural network
class NeuralNetwork {
 public:
  typedef std::vector<Neuron> Layer;
  typedef std::vector<Neuron::Value> ValueVector;

  NeuralNetwork();
  NeuralNetwork(const nlohmann::json& json);
  virtual ~NeuralNetwork() = default;

  // Copy operations
  NeuralNetwork(const NeuralNetwork& other);
  auto operator=(const NeuralNetwork& other) -> NeuralNetwork&;

  // Move operations
  NeuralNetwork(NeuralNetwork&& other) noexcept;
  auto operator=(NeuralNetwork&& other) noexcept -> NeuralNetwork&;

  // Equality operator
  auto operator==(const NeuralNetwork& other) const -> bool;

  auto set_input_count(size_t count) -> void;
  auto get_input_count() const -> size_t;

  auto set_input_values(const ValueVector& input) -> void;
  auto get_input_values() const -> const ValueVector&;

  auto set_output_neuron_count(size_t count) -> void;
  auto get_output_neuron_count() const -> size_t;
  auto get_output_values() -> ValueVector;

  auto get_output_layer() const -> const Layer&;
  auto set_output_layer(const Layer& layer) -> void;

  auto set_hidden_layer_count(size_t count) -> void;
  auto get_hidden_layer_count() const -> size_t;
  auto get_hidden_layer(size_t idx) const -> const Layer&;
  auto set_hidden_layer(size_t idx, const Layer& layer) -> void;

  auto set_hidden_layer_neuron_count(size_t count) -> void;
  auto get_hidden_layer_neuron_count() const -> size_t;

  auto randomize() -> void;

  auto to_json() const -> nlohmann::json;

 protected:
  auto configure_output_layer() -> void;
  auto configure_hidden_layers() -> void;
  auto get_hidden_layer_weight_count(size_t layer_idx) -> size_t;
  auto configure_hidden_layer(size_t idx) -> void;
  auto compute() -> void;
  auto validate() -> void;

  const size_t DEFAULT_HIDDEN_LAYER_COUNT = 2;
  const size_t DEFAULT_HIDDEN_LAYER_NEURON_COUNT = 16;
  const size_t DEFAULT_INPUT_COUNT = 100;
  const size_t DEFAULT_OUTPUT_NEURON_COUNT = 2;

  std::vector<Layer> _hiddenLayers;
  Layer _outputLayer;
  ValueVector _inputsValues;
  ValueVector _outputValues;
  bool _validated = false;

  size_t _hiddenLayerNeuronCount;

  // ready is set to false when inputs change and is set back to true
  // when the output is retrieved and all upstream data gets recalculated
  bool _ready = false;
  RandomGenerator _randomGenerator;
};
