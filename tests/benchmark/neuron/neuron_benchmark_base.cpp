#include "neuron_benchmark_base.hpp"

#include <chrono>
#include <iostream>

#include "random_generator.hpp"

auto NeuronBenchmarkBase::generate_random_inputs() -> void {
  _inputs.resize(NUM_INPUTS);
  for (auto& input : _inputs) {
    input = _randomGenerator.uniform(-1.0F, 1.0F);
  }
}

auto NeuronBenchmarkBase::reset() -> void {
  RandomGenerator generator;
  _neuron.set_input_count(NUM_INPUTS);
  _neuron.randomize(generator);
  generate_random_inputs();
}

auto NeuronBenchmarkBase::derived_run() -> void {
  _neuron.set_inputs(_inputs);
  _neuron.get_output();
}