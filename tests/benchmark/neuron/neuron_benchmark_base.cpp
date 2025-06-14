#include "neuron_benchmark_base.hpp"

#include <chrono>
#include <iostream>

auto NeuronBenchmarkBase::generate_random_inputs() -> void {
  _inputs.resize(NUM_INPUTS);
  for (auto& input : _inputs) {
    input = _randomGenerator.uniform(-1.0F, 1.0F);
  }
}

auto NeuronBenchmarkBase::reset() -> void {
  _neuron.set_input_count(NUM_INPUTS);
  _neuron.randomize();
  generate_random_inputs();
}

auto NeuronBenchmarkBase::run() -> void {
  auto start = std::chrono::high_resolution_clock::now();
  _neuron.set_inputs(_inputs);
  _neuron.get_output();
  auto stop = std::chrono::high_resolution_clock::now();
  _duration += std::chrono::duration<double>(stop - start);
}

auto NeuronBenchmarkBase::get_duration() -> std::chrono::microseconds {
  return std::chrono::duration_cast<std::chrono::microseconds>(_duration);
}

auto NeuronBenchmarkBase::set_name(const std::string& name) -> void {
  _name = name;
}

auto NeuronBenchmarkBase::display() -> void {
  std::cout << _name << ": " << get_duration() << "\n";
}