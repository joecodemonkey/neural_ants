#include "neuron.hpp"

#include <random>

auto Neuron::set_input(size_t idx, Value value) -> void {
  _inputs.at(idx) = value;
  _ready = false;
}

auto Neuron::get_input(size_t idx) const -> Value {
  return _inputs.at(idx);
}

auto Neuron::set_input_weight(size_t idx, Value weight) -> void {
  _weights.at(idx) = weight;
  _ready = false;
}

auto Neuron::get_input_weight(size_t idx) const -> Value {
  return _weights.at(idx);
}

auto Neuron::set_bias(double bias) -> void {
  _bias = static_cast<Value>(bias);
  _ready = false;
}

auto Neuron::get_bias() const -> Value {
  return _bias;
}

auto Neuron::get_output() -> Value {
  if (!_ready) {
    // Calculate weighted sum
    _value = _bias;
    for (size_t i = 0; i < _inputs.size(); ++i) {
      _value += _inputs.at(i) * _weights.at(i);
    }
    // Apply activation function
    _value = activation_function(_value);
    _ready = true;
  }
  return _value;
}

auto Neuron::get_input_count() const -> size_t {
  return _inputs.size();
}

auto Neuron::set_input_count(size_t count) -> void {
  _inputs.resize(count, 0.0f);
  _weights.resize(count, 0.0f);
  _ready = false;
}

auto Neuron::operator=(const Neuron& other) -> Neuron& {
  if (this != &other) {
    _inputs = other._inputs;
    _weights = other._weights;
    _bias = other._bias;
    _value = other._value;
    _ready = other._ready;
  }
  return *this;
}

auto Neuron::operator=(Neuron&& other) -> Neuron& {
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

auto Neuron::randomize() -> void {
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

auto Neuron::get_inputs() const -> const ValueVector& {
  return _inputs;
}

auto Neuron::get_inputs() -> ValueVector& {
  return _inputs;
}