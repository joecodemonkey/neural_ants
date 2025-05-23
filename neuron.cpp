#include "neuron.hpp"
#include <stdexcept>
#include <vector>

void Neuron::set_input(size_t idx, Value value) {
    if (idx >= _inputs.size()) {
        throw std::out_of_range("Input index out of range");
    }
    _inputs[idx] = value;
    _ready = false;
}

Neuron::Value Neuron::get_input(size_t idx) const {
    if (idx >= _inputs.size()) {
        throw std::out_of_range("Input index out of range");
    }
    return _inputs[idx];
}

void Neuron::set_input_weight(size_t idx, Value weight) {
    if (idx >= _weights.size()) {
        throw std::out_of_range("Weight index out of range");
    }
    _weights[idx] = weight;
    _ready = false;
}

Neuron::Value Neuron::get_input_weight(size_t idx) const {
    if (idx >= _weights.size()) {
        throw std::out_of_range("Weight index out of range");
    }
    return _weights[idx];
}

void Neuron::set_bias(double bias) {
    _bias = static_cast<Value>(bias);
    _ready = false;
}

Neuron::Value Neuron::get_bias() const {
    return _bias;
}

Neuron::Value Neuron::get_output() {
    if (!_ready) {
        // Calculate weighted sum
        _value = _bias;
        for (size_t i = 0; i < _inputs.size(); ++i) {
            _value += _inputs[i] * _weights[i];
        }
        // Apply activation function
        _value = activation_function(_value);
        _ready = true;
    }
    return _value;
}

size_t Neuron::get_input_count() const {
    return _inputs.size();
}

void Neuron::set_input_count(size_t count) {
    _inputs.resize(count, 0.0f);
    _weights.resize(count, 0.0f);
    _ready = false;
}

Neuron& Neuron::operator=(const Neuron& other) {
    if (this != &other) {
        _inputs = other._inputs;
        _weights = other._weights;
        _bias = other._bias;
        _value = other._value;
        _ready = other._ready;
    }
    return *this;
}

Neuron& Neuron::operator=(Neuron&& other) {
    if (this != &other) {
        _inputs = std::move(other._inputs);
        _weights = std::move(other._weights);
        _bias = other._bias;
        _value = other._value;
        _ready = other._ready;
    }
    return *this;
}

Neuron::Neuron(const Neuron& other) {
    _inputs = other._inputs;
    _weights = other._weights;
    _bias = other._bias;
    _value = other._value;
    _ready = other._ready;
}

Neuron::Neuron(Neuron&& other) {
    _inputs = std::move(other._inputs);
    _weights = std::move(other._weights);
    _bias = other._bias;
    _value = other._value;
    _ready = other._ready;
} 

void Neuron::randomize() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<Value> dist(-1.0f, 1.0f);

    for (auto& weight : _weights) {
        weight = dist(gen);
    }

    _bias = dist(gen);
    _value = dist(gen);
    _ready = false;
}