#include "neuron.hpp"

#include <algorithm>
#include <execution>
#include <numeric>
#include <random>
#include <ranges>
#include <stdexcept>

auto Neuron::set_input(size_t idx, Value value) -> void {
  _inputs.at(idx) = value;
  calculate();
}

auto Neuron::get_input(size_t idx) const -> Value {
  return _inputs.at(idx);
}

auto Neuron::set_input_weight(size_t idx, Value weight) -> void {
  _weights.at(idx) = weight;
}

auto Neuron::get_input_weight(size_t idx) const -> Value {
  return _weights.at(idx);
}

auto Neuron::set_bias(double bias) -> void {
  _bias = static_cast<Value>(bias);
}

auto Neuron::get_bias() const -> Value {
  return _bias;
}

auto Neuron::get_output() -> Value {
  _value = _bias;
  for (size_t i = 0; i < _inputs.size(); ++i) {
    _value += _inputs.at(i) * _weights.at(i);
  }
  // Apply activation function
  _value = activation_function(_value);

  return _value;
}

auto Neuron::get_input_count() const -> size_t {
  return _inputs.size();
}

auto Neuron::set_input_count(size_t count) -> void {
  _inputs.resize(count, 0.0f);
  _weights.resize(count, 0.0f);
}

auto Neuron::operator=(const Neuron& other) -> Neuron& {
  if (this != &other) {
    _inputs = other._inputs;
    _weights = other._weights;
    _bias = other._bias;
    _value = other._value;
  }
  return *this;
}

auto Neuron::operator=(Neuron&& other) -> Neuron& {
  if (this != &other) {
    _inputs = std::move(other._inputs);
    _weights = std::move(other._weights);
    _bias = other._bias;
    _value = other._value;
  }
  return *this;
}

Neuron::Neuron(const Neuron& other) {
  _inputs = other._inputs;
  _weights = other._weights;
  _bias = other._bias;
  _value = other._value;
}

Neuron::Neuron(Neuron&& other) {
  _inputs = std::move(other._inputs);
  _weights = std::move(other._weights);
  _bias = other._bias;
  _value = other._value;
}

Neuron::Neuron(const nlohmann::json& json) {
  _bias = json.at("bias").get<Value>();
  _weights = json.at("weights").get<ValueVector>();
  _inputs = json.at("inputs").get<ValueVector>();
  _value = json.at("value").get<Value>();
  _threaded = json.at("threaded").get<bool>();
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
}

auto Neuron::get_inputs() const -> const ValueVector& {
  return _inputs;
}

auto Neuron::set_inputs(const ValueVector& inputs) -> void {
  if (inputs.size() != _inputs.size()) {
    throw std::runtime_error("Input size mismatch: expected " + std::to_string(_inputs.size()) +
                             " but got " + std::to_string(inputs.size()));
  }
  _inputs = inputs;
  calculate();
}

auto Neuron::enable_threads() -> void {
  _threaded = true;
}

auto Neuron::disable_threads() -> void {
  _threaded = false;
}

auto Neuron::calculate() -> void {
  double _value;
  if (_threaded) {
    _value = std::transform_reduce(std::execution::par,
                                   _inputs.begin(),
                                   _inputs.end(),
                                   _weights.begin(),
                                   0.0,
                                   std::plus<double>{},
                                   std::multiplies<double>{});
  } else {
    _value = std::transform_reduce(std::execution::seq,
                                   _inputs.begin(),
                                   _inputs.end(),
                                   _weights.begin(),
                                   0.0,
                                   std::plus<double>{},
                                   std::multiplies<double>{});
  }

  _value = activation_function(_bias + _value);
}

auto Neuron::to_json() const -> nlohmann::json {
  nlohmann::json json;
  json["bias"] = _bias;
  json["weights"] = _weights;
  json["inputs"] = _inputs;
  json["value"] = _value;
  json["threaded"] = _threaded;
  return json;
}