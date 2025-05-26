#include "neural_network.hpp"

#include <algorithm>
#include <random>
#include <stdexcept>

auto NeuralNetwork::set_hidden_layer_count(size_t count) -> void {
  _layers.resize(count);
  _ready = false;
}

auto NeuralNetwork::get_hidden_layer_count() const -> size_t {
  return _layers.size();
}

auto NeuralNetwork::set_input(const ValueVector& input) -> void {
  if (input.size() != _inputs.size()) {
    throw std::invalid_argument("Input size mismatch");
  }
  _inputs = input;
  _ready = false;
}

auto NeuralNetwork::set_input(size_t idx, Neuron::Value value) -> void {
  _inputs.at(idx) = value;
  _ready = false;
}

auto NeuralNetwork::get_inputs() const -> const ValueVector& {
  return _inputs;
}

auto NeuralNetwork::get_input(size_t idx) const -> Neuron::Value {
  return _inputs.at(idx);
}

auto NeuralNetwork::get_outputs() -> const ValueVector& {
  if (!_ready) {
    // Forward propagate through the network
    ValueVector current_inputs = _inputs;

    // Process hidden layers
    for (auto& layer : _layers) {
      ValueVector layer_outputs;
      layer_outputs.reserve(layer.size());

      for (Neuron& neuron : layer) {
        // Ensure neuron has correct number of inputs
        if (neuron.get_input_count() != current_inputs.size()) {
          neuron.set_input_count(current_inputs.size());
        }

        // Set inputs for this neuron
        for (size_t i = 0; i < current_inputs.size(); ++i) {
          neuron.set_input(i, current_inputs[i]);
        }
        layer_outputs.push_back(neuron.get_output());
      }

      current_inputs = layer_outputs;
    }

    // Process output layer
    _outputs.clear();
    _outputs.reserve(_output_layer.size());

    for (Neuron& neuron : _output_layer) {
      // Ensure neuron has correct number of inputs
      if (neuron.get_input_count() != current_inputs.size()) {
        neuron.set_input_count(current_inputs.size());
      }

      // Set inputs for this neuron
      for (size_t i = 0; i < current_inputs.size(); ++i) {
        neuron.set_input(i, current_inputs[i]);
      }
      _outputs.push_back(neuron.get_output());
    }

    _ready = true;
  }
  return _outputs;
}

auto NeuralNetwork::get_output(size_t idx) -> Neuron::Value {
  return get_outputs().at(idx);
}

auto NeuralNetwork::set_input_layer_count(size_t count) -> void {
  _inputs.resize(count, 0.0f);
  _ready = false;
}

auto NeuralNetwork::get_input_layer_count() const -> size_t {
  return _inputs.size();
}

auto NeuralNetwork::set_output_layer_count(size_t count) -> void {
  _output_layer.resize(count);
  _outputs.resize(count, 0.0f);
  _ready = false;
}

auto NeuralNetwork::get_output_layer_count() const -> size_t {
  return _output_layer.size();
}

auto NeuralNetwork::get_layer(size_t index) -> Layer& {
  return _layers.at(index);
}

auto NeuralNetwork::get_layer(size_t index) const -> const Layer& {
  return _layers.at(index);
}

auto NeuralNetwork::get_output_layer() -> Layer& {
  return _output_layer;
}

auto NeuralNetwork::get_output_layer() const -> const Layer& {
  return _output_layer;
}

auto NeuralNetwork::randomize() -> void {
  // Randomize hidden layers
  for (size_t layer_idx = 0; layer_idx < _layers.size(); ++layer_idx) {
    auto& layer = _layers[layer_idx];

    // First layer gets inputs from network inputs
    // Other layers get inputs from previous layer
    size_t input_count = (layer_idx == 0) ? _inputs.size() : _layers[layer_idx - 1].size();

    for (auto& neuron : layer) {
      neuron.set_input_count(input_count);
      neuron.randomize();
    }
  }

  // Randomize output layer
  size_t output_input_count = _layers.empty() ? _inputs.size() : _layers.back().size();
  for (auto& neuron : _output_layer) {
    neuron.set_input_count(output_input_count);
    neuron.randomize();
  }

  _ready = false;
}