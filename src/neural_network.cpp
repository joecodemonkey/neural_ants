#include "neural_network.hpp"

#include <algorithm>
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

// Equality operator
auto NeuralNetwork::operator==(const NeuralNetwork& other) const -> bool {
  return _hiddenLayerNeuronCount == other._hiddenLayerNeuronCount &&
         _validated == other._validated && _ready == other._ready &&
         _inputsValues == other._inputsValues && _outputValues == other._outputValues &&
         _hiddenLayers == other._hiddenLayers && _outputLayer == other._outputLayer;
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
  // Randomize hidden layers
  for (auto& layer : _hiddenLayers) {
    for (auto& neuron : layer) {
      neuron.randomize(_randomGenerator);
    }
  }

  // Randomize output layer
  for (auto& neuron : _outputLayer) {
    neuron.randomize(_randomGenerator);
  }

  _ready = false;
}

auto NeuralNetwork::compute() -> void {
  if (!_validated) {
    validate();
  }

  ValueVector inputs;
  _outputValues.clear();
  _outputValues.resize(_outputLayer.size());

  if (_hiddenLayers.empty()) {
    inputs = get_input_values();
  } else {
    inputs = get_input_values();
    ValueVector outputs;
    outputs.reserve(_hiddenLayerNeuronCount);
    for (size_t layerIndex = 0; layerIndex < _hiddenLayers.size(); ++layerIndex) {
      outputs.clear();
      for (size_t neuronIndex = 0; neuronIndex < _hiddenLayerNeuronCount; ++neuronIndex) {
        _hiddenLayers[layerIndex][neuronIndex].set_inputs(inputs);
        outputs.push_back(_hiddenLayers[layerIndex][neuronIndex].get_output());
      }
      inputs = outputs;
    }
  }

  for (size_t neuronIndex = 0; neuronIndex < _outputLayer.size(); ++neuronIndex) {
    _outputLayer[neuronIndex].set_inputs(inputs);
    _outputValues[neuronIndex] = _outputLayer[neuronIndex].get_output();
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

auto NeuralNetwork::get_output_values() -> ValueVector {
  if (!_ready) {
    this->compute();
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

auto NeuralNetwork::to_json() const -> nlohmann::json {
  nlohmann::json json;

  json["input_count"] = _inputsValues.size();
  json["hidden_layer_count"] = _hiddenLayers.size();
  json["hidden_layer_neuron_count"] = _hiddenLayerNeuronCount;
  json["output_neuron_count"] = _outputLayer.size();
  json["validated"] = _validated;
  json["ready"] = _ready;

  json["input_values"] = _inputsValues;
  json["output_values"] = _outputValues;

  nlohmann::json hidden_layers_json = nlohmann::json::array();
  for (const auto& layer : _hiddenLayers) {
    nlohmann::json layer_json = nlohmann::json::array();
    for (const auto& neuron : layer) {
      layer_json.push_back(neuron.to_json());
    }
    hidden_layers_json.push_back(layer_json);
  }
  json["hidden_layers"] = hidden_layers_json;

  nlohmann::json output_layer_json = nlohmann::json::array();
  for (const auto& neuron : _outputLayer) {
    output_layer_json.push_back(neuron.to_json());
  }
  json["output_layer"] = output_layer_json;

  return json;
}

NeuralNetwork::NeuralNetwork(const nlohmann::json& json) {
  _hiddenLayerNeuronCount = json.at("hidden_layer_neuron_count").get<size_t>();
  _validated = json.at("validated").get<bool>();
  _ready = json.at("ready").get<bool>();

  _inputsValues = json.at("input_values").get<ValueVector>();
  _outputValues = json.at("output_values").get<ValueVector>();

  _hiddenLayers.clear();
  for (const auto& layer_json : json.at("hidden_layers")) {
    Layer layer;
    for (const auto& neuron_json : layer_json) {
      layer.push_back(Neuron(neuron_json));
    }
    _hiddenLayers.push_back(layer);
  }

  _outputLayer.clear();
  for (const auto& neuron_json : json.at("output_layer")) {
    _outputLayer.push_back(Neuron(neuron_json));
  }
}