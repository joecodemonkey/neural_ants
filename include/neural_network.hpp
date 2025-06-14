#pragma once
#include <vector>

#include "neuron.hpp"

// A fully connected neural network
class NeuralNetwork {
 public:
  typedef std::vector<Neuron> Layer;
  typedef std::vector<Neuron::Value> ValueVector;

  NeuralNetwork();
  virtual ~NeuralNetwork() = default;

  // Copy operations
  NeuralNetwork(const NeuralNetwork& other);
  auto operator=(const NeuralNetwork& other) -> NeuralNetwork&;

  // Move operations
  NeuralNetwork(NeuralNetwork&& other) noexcept;
  auto operator=(NeuralNetwork&& other) noexcept -> NeuralNetwork&;

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
  auto enable_threads() -> void;
  auto disable_threads() -> void;

 protected:
  auto configure_output_layer() -> void;
  auto configure_hidden_layers() -> void;
  auto get_hidden_layer_weight_count(size_t layer_idx) -> size_t;
  auto configure_hidden_layer(size_t idx) -> void;
  auto get_hidden_layer_values(size_t layerIndex) -> ValueVector;
  auto compute() -> void;
  auto validate() -> void;

  const size_t DEFAULT_HIDDEN_LAYER_COUNT = 3;
  const size_t DEFAULT_HIDDEN_LAYER_NEURON_COUNT = 50;
  const size_t DEFAULT_INPUT_COUNT = 50;
  const size_t DEFAULT_OUTPUT_NEURON_COUNT = 2;

  std::vector<Layer> _hiddenLayers;
  Layer _outputLayer;
  ValueVector _inputsValues;
  ValueVector _outputValues;
  bool _validated = false;
  bool _threaded = false;

  size_t _hiddenLayerNeuronCount;

  // ready is set to false when inputs change and is set back to true
  // when the output is retrieved and all upstream data gets recalculated
  bool _ready = false;
};
