#include "genome.hpp"

#include "neural_network.hpp"
#include "neuron.hpp"

Genome::Genome(const Genome& other)
    : _network(other._network), _mutationRate(other._mutationRate), _fitness(other._fitness) {}

Genome::Genome(Genome&& other) noexcept
    : _network(std::move(other._network)),
      _mutationRate(other._mutationRate),
      _fitness(other._fitness) {}

auto Genome::operator=(const Genome& other) -> Genome& {
  if (this != &other) {
    _network = other._network;
    _mutationRate = other._mutationRate;
    _fitness = other._fitness;
  }
  return *this;
}

auto Genome::operator=(Genome&& other) noexcept -> Genome& {
  if (this != &other) {
    _network = std::move(other._network);
    _mutationRate = other._mutationRate;
    _fitness = other._fitness;
  }
  return *this;
}

auto Genome::get_network() const -> const NeuralNetwork& {
  return _network;
}

auto Genome::breed_neuron(const Neuron& parentNeuron, Neuron& childNeuron) -> void {
  for (size_t weightIdx = 0; weightIdx < parentNeuron.get_input_count(); ++weightIdx) {
    if (_rng.coin_flip()) {
      childNeuron.set_input_weight(weightIdx, parentNeuron.get_input_weight(weightIdx));
    }
  }

  if (_rng.coin_flip()) {
    childNeuron.set_bias(parentNeuron.get_bias());
  }
}

auto Genome::breed_layer(const NeuralNetwork::Layer& parent1, NeuralNetwork::Layer& child) -> void {
  for (size_t neuronIdx = 0; neuronIdx < parent1.size(); ++neuronIdx) {
    const Neuron& parent1Neuron = parent1[neuronIdx];
    Neuron& childNeuron = child[neuronIdx];
    breed_neuron(parent1Neuron, childNeuron);
  }
}

auto Genome::mutate_neuron(Neuron& neuron) {
  if (should_mutate()) {
    neuron.set_bias(neuron.get_bias() + mutation_amount());
  }
  for (size_t index = 0; index < neuron.get_input_count(); ++index) {
    if (should_mutate()) {
      neuron.set_input_weight(index, neuron.get_input_weight(index) + mutation_amount());
    }
  }
}

auto Genome::mutate_layer(NeuralNetwork::Layer& layer) {
  for (Neuron& neuron : layer) {
    mutate_neuron(neuron);
  }
}

auto Genome::breed_network(NeuralNetwork& childNetwork) -> void {
  // Breed hidden layers
  for (size_t layerIdx = 0; layerIdx < _network.get_hidden_layer_count(); ++layerIdx) {
    auto layer = childNetwork.get_hidden_layer(layerIdx);
    breed_layer(_network.get_hidden_layer(layerIdx), layer);
    childNetwork.set_hidden_layer(layerIdx, layer);
  }

  auto layer = childNetwork.get_output_layer();
  breed_layer(_network.get_output_layer(), layer);
  childNetwork.set_output_layer(layer);
}

auto Genome::breed_with(const Genome& other) -> Genome {
  Genome child = other;  // child's genome begins as a clone of parent2 which simplifies later logic
  breed_network(child._network);
  child.mutate();
  return child;
}

auto Genome::mutate() -> void {
  for (size_t layerIdx = 0; layerIdx < _network.get_hidden_layer_count(); ++layerIdx) {
    auto layer = _network.get_hidden_layer(layerIdx);
    mutate_layer(layer);
    _network.set_hidden_layer(layerIdx, layer);
  }
  auto layer = _network.get_output_layer();
  mutate_layer(layer);
  _network.set_output_layer(layer);
}

auto Genome::randomize() -> void {
  _network.randomize();
}

auto Genome::should_mutate() -> bool {
  return (_mutationRate < _rng.normal(0.0F, 1.0F));
}
auto Genome::mutation_amount() -> double {
  return _rng.normal(0.0F, 0.1F);
}

auto Genome::set_mutation_rate(double mutationRate) -> void {
  _mutationRate = mutationRate;
}
auto Genome::get_mutation_rate() const -> double {
  return _mutationRate;
}

auto Genome::get_fitness() const -> double {
  return _fitness;
}

auto Genome::set_fitness(double fitness) -> void {
  _fitness = fitness;
}