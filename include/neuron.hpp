#pragma once
#include <cmath>
#include <vector>

class Neuron {
 public:
  typedef float Value;
  typedef std::vector<Value> ValueVector;

  Neuron() {}

  auto set_input(size_t idx, Value value) -> void;
  auto get_input(size_t idx) const -> Value;
  auto get_inputs() const -> const ValueVector&;
  auto set_inputs(const ValueVector& inputs) -> void;

  auto set_input_weight(size_t idx, Value weight) -> void;
  auto get_input_weight(size_t idx) const -> Value;

  auto set_bias(double bias) -> void;
  auto get_bias() const -> Value;

  auto get_output() -> Value;

  auto get_input_count() const -> size_t;
  auto set_input_count(size_t) -> void;

  auto operator=(const Neuron& other) -> Neuron&;
  auto operator=(Neuron&& other) -> Neuron&;
  Neuron(const Neuron& other);
  Neuron(Neuron&& other);

  auto randomize() -> void;

 protected:
  static auto activation_function(Value x) -> Value {
    return tanh(x);
  }

  bool _ready = false;

  Value _value = 0;
  Value _bias = 0;
  ValueVector _weights;
  ValueVector _inputs;
};
