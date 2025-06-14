#include "neural_network_benchmark_base.hpp"

auto NeuralNetworkBenchmarkBase::generate_random_inputs() -> void {
  _inputs.resize(INPUT_LAYER_SIZE);
  for (auto& input : _inputs) {
    input = _randomGenerator.uniform(-1.0F, 1.0F);
  }
}

auto NeuralNetworkBenchmarkBase::setup_network() -> void {
  _network.set_input_count(INPUT_LAYER_SIZE);
  _network.set_hidden_layer_neuron_count(HIDDEN_LAYER_SIZE);
  _network.set_output_neuron_count(OUTPUT_LAYER_SIZE);
  _network.randomize();
}

auto NeuralNetworkBenchmarkBase::reset() -> void {
  setup_network();
  generate_random_inputs();
}

auto NeuralNetworkBenchmarkBase::derived_run() -> void {
  _network.set_input_values(_inputs);
  _network.get_output_values();
}