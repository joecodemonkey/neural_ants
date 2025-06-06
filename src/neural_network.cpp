#include "neural_network.hpp"

#include <random>
#include <stdexcept>

#include "neuron.hpp"

NeuralNetwork::NeuralNetwork() {
  _hiddenLayerNeuronCount = DEFAULT_HIDDEN_LAYER_COUNT;
  set_input_count(DEFAULT_INPUT_COUNT);
  set_hidden_layer_neuron_count(DEFAULT_HIDDEN_LAYER_NEURON_COUNT);
  set_hidden_layer_count(DEFAULT_HIDDEN_LAYER_COUNT);
  set_output_neuron_count(DEFAULT_OUTPUT_NEURON_COUNT);
  _validated = true;
}
// Copy constructor
NeuralNetwork::NeuralNetwork(const NeuralNetwork& other)
    : _hiddenLayers(other._hiddenLayers),
      _outputLayer(other._outputLayer),
      _inputsValues(other._inputsValues),
      _ready(other._ready),
      _hiddenLayerNeuronCount(other._hiddenLayerNeuronCount),
      _validated(other._validated) {}

// Copy assignment
auto NeuralNetwork::operator=(const NeuralNetwork& other) -> NeuralNetwork& {
  if (this != &other) {
    _hiddenLayers = other._hiddenLayers;
    _outputLayer = other._outputLayer;
    _inputsValues = other._inputsValues;
    _ready = other._ready;
    _validated = other._validated;
    _hiddenLayerNeuronCount = other._hiddenLayerNeuronCount;
  }
  return *this;
}

// Move constructor
NeuralNetwork::NeuralNetwork(NeuralNetwork&& other) noexcept
    : _hiddenLayers(std::move(other._hiddenLayers)),
      _outputLayer(std::move(other._outputLayer)),
      _inputsValues(std::move(other._inputsValues)),
      _ready(other._ready),
      _validated(other._validated),
      _hiddenLayerNeuronCount(other._hiddenLayerNeuronCount) {}

// Move assignment
auto NeuralNetwork::operator=(NeuralNetwork&& other) noexcept -> NeuralNetwork& {
  if (this != &other) {
    _hiddenLayers = std::move(other._hiddenLayers);
    _outputLayer = std::move(other._outputLayer);
    _inputsValues = std::move(other._inputsValues);
    _ready = other._ready;
    _hiddenLayerNeuronCount = other._hiddenLayerNeuronCount;
    _validated = other._validated;
  }
  return *this;
}

auto NeuralNetwork::set_input_values(const ValueVector& input) -> void {
  _inputsValues = input;
  _ready = false;
}

auto NeuralNetwork::set_input_count(size_t count) -> void {
  _inputsValues.resize(count, 0.0f);
  if (_hiddenLayers.size() > 0) {
    configure_hidden_layer(0);
  }
  _ready = false;
}

auto NeuralNetwork::get_input_count() const -> size_t {
  return _inputsValues.size();
}

auto NeuralNetwork::get_input_values() const -> const ValueVector& {
  return _inputsValues;
}

auto NeuralNetwork::get_output_layer() const -> const Layer& {
  return _outputLayer;
}

auto NeuralNetwork::configure_output_layer() -> void {
  for (Neuron& neuron : _outputLayer) {
    if (_hiddenLayers.empty()) {
      neuron.set_input_count(get_input_count());
    } else {
      neuron.set_input_count(_hiddenLayerNeuronCount);
    }
  }
}

auto NeuralNetwork::set_output_neuron_count(size_t count) -> void {
  _outputLayer.resize(count);
  configure_output_layer();
  _ready = false;
}

auto NeuralNetwork::get_output_neuron_count() const -> size_t {
  return _outputLayer.size();
}

auto NeuralNetwork::get_hidden_layer(size_t idx) const -> const Layer& {
  return _hiddenLayers.at(idx);
}

auto NeuralNetwork::get_hidden_layer_weight_count(size_t layerIndex) -> size_t {
  if (layerIndex == 0) {
    return _inputsValues.size();
  }

  return _hiddenLayerNeuronCount;
}

auto NeuralNetwork::configure_hidden_layer(size_t idx) -> void {
  Layer& layer = _hiddenLayers.at(idx);
  layer.resize(_hiddenLayerNeuronCount);
  for (Neuron& neuron : layer) {
    if (idx == 0) {
      neuron.set_input_count(_inputsValues.size());
    } else {
      neuron.set_input_count(_hiddenLayerNeuronCount);
    }
  }
}
auto NeuralNetwork::configure_hidden_layers() -> void {
  for (size_t layerIndex = 0; layerIndex < _hiddenLayers.size(); ++layerIndex) {
    configure_hidden_layer(layerIndex);
  }
}

auto NeuralNetwork::set_hidden_layer_count(size_t count) -> void {
  _hiddenLayers.resize(count);
  configure_hidden_layers();
  _ready = false;
}

auto NeuralNetwork::get_hidden_layer_count() const -> size_t {
  return _hiddenLayers.size();
}

auto NeuralNetwork::set_hidden_layer_neuron_count(size_t count) -> void {
  _hiddenLayerNeuronCount = count;
  configure_hidden_layers();
  configure_output_layer();
  _ready = false;
}

auto NeuralNetwork::get_hidden_layer_neuron_count() const -> size_t {
  return _hiddenLayerNeuronCount;
}

auto NeuralNetwork::randomize() -> void {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<Neuron::Value> dist(-1.0f, 1.0f);

  // Randomize hidden layers
  for (auto& layer : _hiddenLayers) {
    for (auto& neuron : layer) {
      neuron.randomize();
    }
  }

  // Randomize output layer
  for (auto& neuron : _outputLayer) {
    neuron.randomize();
  }

  _ready = false;
}

auto NeuralNetwork::get_hidden_layer_values(size_t layerIndex) -> ValueVector {
  auto& layer = _hiddenLayers.at(layerIndex);
  ValueVector valueVector;
  valueVector.reserve(_hiddenLayerNeuronCount);

  ValueVector inputs;

  if (layerIndex == 0) {
    inputs = get_input_values();
  } else {
    inputs = get_hidden_layer_values(layerIndex - 1);
  }

  for (auto& neuron : layer) {
    neuron.set_inputs(inputs);
    valueVector.push_back(neuron.get_output());
  }

  return valueVector;
}

auto NeuralNetwork::compute() -> void {
  if (!_validated) {
    validate();
  }

  ValueVector inputs;

  if (_hiddenLayers.empty()) {
    inputs = get_input_values();
  } else {
    inputs = get_hidden_layer_values(_hiddenLayers.size() - 1);
  }

  _outputValues.clear();
  _outputValues.reserve(_outputLayer.size());

  for (Neuron& neuron : _outputLayer) {
    neuron.set_inputs(inputs);
    _outputValues.push_back(neuron.get_output());
  }
  _ready = true;
}

auto NeuralNetwork::validate() -> void {
  if (_inputsValues.empty()) {
    throw std::runtime_error("Input values are empty - cannot compute neural network");
  }
  if (_outputLayer.empty()) {
    throw std::runtime_error("There are no Output Neurons - cannot compute neural network");
  }
  if (_hiddenLayers.empty()) {
    for (const Neuron& neuron : _outputLayer) {
      if (neuron.get_input_count() != get_input_count()) {
        throw std::runtime_error("Output neuron input count does not match network input count");
      }
    }
    _validated = true;
    return;
  }
  for (size_t layerIdx = 0; auto& layer : _hiddenLayers) {
    if (layer.size() != _hiddenLayerNeuronCount) {
      throw std::runtime_error(
          "Hidden layer " + std::to_string(layerIdx) + " neuron count mismatch - expected " +
          std::to_string(_hiddenLayerNeuronCount) + " but got " + std::to_string(layer.size()));
    }
    if (layerIdx == 0) {
      for (const Neuron& neuron : layer) {
        if (neuron.get_input_count() != _inputsValues.size()) {
          throw std::runtime_error(
              "First Hidden layer neuron input count does not match network input count");
        }
      }
      continue;
    } else {
      for (const Neuron& neuron : layer) {
        if (neuron.get_input_count() != _hiddenLayerNeuronCount) {
          throw std::runtime_error(
              "Hidden layer " + std::to_string(layerIdx) +
              " neuron input count does not match previous layer neuron count");
        }
      }
    }
  }
  for (const Neuron& neuron : _outputLayer) {
    if (neuron.get_input_count() != _hiddenLayerNeuronCount) {
      throw std::runtime_error(
          "Output neuron input count does not match network hidden layer neuron count");
    }
  }
  _validated = true;
}

auto NeuralNetwork::get_output_values() const -> ValueVector {
  if (!_ready) {
    const_cast<NeuralNetwork*>(this)->compute();
  }
  return _outputValues;
}

auto NeuralNetwork::set_output_layer(const Layer& layer) -> void {
  _outputLayer = layer;
  configure_output_layer();
  _ready = false;
}

auto NeuralNetwork::set_hidden_layer(size_t idx, const Layer& layer) -> void {
  if (idx >= _hiddenLayers.size()) {
    throw std::runtime_error("Hidden layer index out of bounds");
  }
  if (layer.size() != get_hidden_layer_weight_count(idx)) {
    throw std::runtime_error("Hidden layer " + std::to_string(idx) + " size mismatch - expected " +
                             std::to_string(get_hidden_layer_weight_count(idx)) + " but got " +
                             std::to_string(layer.size()));
  }
  _hiddenLayers[idx] = layer;
  _ready = false;
}