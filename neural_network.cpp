#include "neural_network.hpp"
#include <stdexcept>
#include <random>
#include <algorithm>

void NeuralNetwork::set_hidden_layer_count(size_t count) {
    _layers.resize(count);
    _ready = false;
}

size_t NeuralNetwork::get_hidden_layer_count() const {
    return _layers.size();
}

void NeuralNetwork::set_input(const ValueVector& input) {
    if (input.size() != _inputs.size()) {
        throw std::invalid_argument("Input size mismatch");
    }
    _inputs = input;
    _ready = false;
}

void NeuralNetwork::set_input(size_t idx, Neuron::Value value) {
    if (idx >= _inputs.size()) {
        throw std::out_of_range("Input index out of range");
    }
    _inputs[idx] = value;
    _ready = false;
}

const NeuralNetwork::ValueVector& NeuralNetwork::get_inputs() const {
    return _inputs;
}

Neuron::Value NeuralNetwork::get_input(size_t idx) const {
    if (idx >= _inputs.size()) {
        throw std::out_of_range("Input index out of range");
    }
    return _inputs[idx];
}

const NeuralNetwork::ValueVector& NeuralNetwork::get_outputs() {
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

Neuron::Value NeuralNetwork::get_output(size_t idx) {
    if (idx >= _outputs.size()) {
        throw std::out_of_range("Output index out of range");
    }
    return get_outputs()[idx];
}

void NeuralNetwork::set_input_layer_count(size_t count) {
    _inputs.resize(count, 0.0f);
    _ready = false;
}

size_t NeuralNetwork::get_input_layer_count() const {
    return _inputs.size();
}

void NeuralNetwork::set_output_layer_count(size_t count) {
    _output_layer.resize(count);
    _outputs.resize(count, 0.0f);
    _ready = false;
}

size_t NeuralNetwork::get_output_layer_count() const {
    return _output_layer.size();
}

NeuralNetwork::Layer& NeuralNetwork::get_layer(size_t index) {
    if (index >= _layers.size()) {
        throw std::out_of_range("Layer index out of range");
    }
    return _layers[index];
}

const NeuralNetwork::Layer& NeuralNetwork::get_layer(size_t index) const {
    if (index >= _layers.size()) {
        throw std::out_of_range("Layer index out of range");
    }
    return _layers[index];
}

NeuralNetwork::Layer& NeuralNetwork::get_output_layer() {
    return _output_layer;
}

const NeuralNetwork::Layer& NeuralNetwork::get_output_layer() const {
    return _output_layer;
}

void NeuralNetwork::randomize() {
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