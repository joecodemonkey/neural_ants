#include "neuron.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <ranges>
#include <stdexcept>
#include <util/math.hpp>

auto Neuron::set_input(size_t idx, Value value) -> void {
  _inputs.at(idx) = value;
  _outputDirty = true;
}

auto Neuron::get_input(size_t idx) const -> Value {
  return _inputs.at(idx);
}

auto Neuron::set_input_weight(size_t idx, Value weight) -> void {
  _weights.at(idx) = weight;
  _outputDirty = true;
}

auto Neuron::get_input_weight(size_t idx) const -> Value {
  return _weights.at(idx);
}

auto Neuron::set_bias(double bias) -> void {
  _bias = static_cast<Value>(bias);
  _outputDirty = true;
}

auto Neuron::get_bias() const -> Value {
  return _bias;
}

auto Neuron::get_output() -> Value {
  if (_outputDirty) {
    Value sum = std::transform_reduce(_inputs.begin(),
                                      _inputs.end(),
                                      _weights.begin(),
                                      0.0f,
                                      std::plus<Value>{},
                                      std::multiplies<Value>{});
    _value = activation_function(_bias + sum);
    _outputDirty = false;
  }
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
    _outputDirty = other._outputDirty;
  }
  return *this;
}

auto Neuron::operator=(Neuron&& other) -> Neuron& {
  if (this != &other) {
    _inputs = std::move(other._inputs);
    _weights = std::move(other._weights);
    _bias = other._bias;
    _value = other._value;
    _outputDirty = other._outputDirty;
  }
  return *this;
}

Neuron::Neuron(const Neuron& other) {
  _inputs = other._inputs;
  _weights = other._weights;
  _bias = other._bias;
  _value = other._value;
  _outputDirty = other._outputDirty;
}

Neuron::Neuron(Neuron&& other) {
  _inputs = std::move(other._inputs);
  _weights = std::move(other._weights);
  _bias = other._bias;
  _value = other._value;
  _outputDirty = other._outputDirty;
}

Neuron::Neuron(const nlohmann::json& json) {
  _bias = json.at("bias").get<Value>();
  _weights = json.at("weights").get<ValueVector>();
  _inputs = json.at("inputs").get<ValueVector>();
  _value = json.at("value").get<Value>();
  _outputDirty = true;
}

auto Neuron::randomize(RandomGenerator& rng) -> void {
  for (auto& weight : _weights) {
    weight = static_cast<Value>(rng.uniform(-1.0, 1.0));
  }

  _bias = static_cast<Value>(rng.uniform(-1.0, 1.0));
  _value = static_cast<Value>(rng.uniform(-1.0, 1.0));
  _outputDirty = true;
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
  _outputDirty = true;
}

auto Neuron::to_json() const -> nlohmann::json {
  nlohmann::json json;
  json["bias"] = _bias;
  json["weights"] = _weights;
  json["inputs"] = _inputs;
  json["value"] = _value;
  return json;
}

auto Neuron::operator==(const Neuron& other) const -> bool {
  if (!Util::equal(_bias, other._bias)) {
    return false;
  }

  if (!Util::equal(_value, other._value)) {
    return false;
  }

  if (!Util::equal(_inputs, other._inputs)) {
    return false;
  }

  return Util::equal(_weights, other._weights);
}