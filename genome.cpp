#include "genome.hpp"

#include <algorithm>
#include <random>

auto Genome::create_network() -> void {
  _network.set_input_layer_count(_inputCount);
  _network.set_output_layer_count(_outputCount);
  _network.set_hidden_layer_count(_layerCount);

  for (size_t i = 0; i < _layerCount; ++i) {
    _network.get_layer(i).resize(_neuronCount);
  }

  _ready = true;
}

Genome::Genome() {}

// Copy constructor
Genome::Genome(const Genome& other)
    : _network(other._network),
      _ready(other._ready),
      _inputCount(other._inputCount),
      _outputCount(other._outputCount),
      _layerCount(other._layerCount),
      _neuronCount(other._neuronCount) {}

// Move constructor
Genome::Genome(Genome&& other) noexcept
    : _network(std::move(other._network)),
      _ready(other._ready),
      _inputCount(other._inputCount),
      _outputCount(other._outputCount),
      _layerCount(other._layerCount),
      _neuronCount(other._neuronCount) {}

// Copy assignment
auto Genome::operator=(const Genome& other) -> Genome& {
  if (this != &other) {
    _network = other._network;
    _ready = other._ready;
    _inputCount = other._inputCount;
    _outputCount = other._outputCount;
    _layerCount = other._layerCount;
    _neuronCount = other._neuronCount;
  }
  return *this;
}

// Move assignment
auto Genome::operator=(Genome&& other) noexcept -> Genome& {
  if (this != &other) {
    _network = std::move(other._network);
    _ready = other._ready;
    _inputCount = other._inputCount;
    _outputCount = other._outputCount;
    _layerCount = other._layerCount;
    _neuronCount = other._neuronCount;
  }
  return *this;
}

auto Genome::get_network() -> NeuralNetwork& {
  if (!_ready) {
    create_network();
  }
  return _network;
}

auto Genome::breed_layer(const NeuralNetwork::Layer& parent1,
                         const NeuralNetwork::Layer& parent2,
                         NeuralNetwork::Layer& child) -> void {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<double> dis(0.0, 1.0);

  for (size_t neuronIdx = 0; neuronIdx < parent1.size(); ++neuronIdx) {
    const Neuron& parent1Neuron = parent1[neuronIdx];
    const Neuron& parent2Neuron = parent2[neuronIdx];
    Neuron& childNeuron = child[neuronIdx];

    // Randomly select weights from either parent
    for (size_t weightIdx = 0; weightIdx < parent1Neuron.get_input_count(); ++weightIdx) {
      Neuron::Value value = 0.0f;
      if (dis(gen) < 0.5) {
        value = parent1Neuron.get_input_weight(weightIdx);
      } else {
        value = parent2Neuron.get_input_weight(weightIdx);
      }
      childNeuron.set_input_weight(weightIdx, value);
    }

    // Randomly select bias from either parent
    if (dis(gen) < 0.5) {
      childNeuron.set_bias(parent1Neuron.get_bias());
    } else {
      childNeuron.set_bias(parent2Neuron.get_bias());
    }
  }
}

auto Genome::breed_with(const Genome& other, double mutationRate) -> Genome {
  validate_compatible(other);

  Genome child;

  // Ensure child has same structure as parents
  child.set_input_count(get_input_count());
  child.set_output_count(get_output_count());
  child.set_layer_count(get_layer_count());
  child.set_hidden_layer_neuron_count(get_hidden_layer_neuron_count());

  // Breed hidden layers
  for (size_t layerIdx = 0; layerIdx < get_layer_count(); ++layerIdx) {
    breed_layer(_network.get_layer(layerIdx),
                other._network.get_layer(layerIdx),
                child._network.get_layer(layerIdx));
  }

  // Breed output layer
  breed_layer(_network.get_output_layer(),
              other._network.get_output_layer(),
              child._network.get_output_layer());

  // Apply mutations
  child.mutate(mutationRate);

  return child;
}

auto Genome::mutate(double mutationRate) -> void {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<double> dis(0.0, 1.0);
  static std::normal_distribution<double> gaussian(0.0, 0.1);  // Small mutations

  // Mutate hidden layers
  for (size_t layerIdx = 0; layerIdx < get_layer_count(); ++layerIdx) {
    auto& layer = _network.get_layer(layerIdx);
    for (auto& neuron : layer) {
      // Mutate weights
      for (size_t weightIdx = 0; weightIdx < neuron.get_input_count(); ++weightIdx) {
        if (dis(gen) < mutationRate) {
          double currentWeight = neuron.get_input_weight(weightIdx);
          neuron.set_input_weight(weightIdx, currentWeight + gaussian(gen));
        }
      }

      // Mutate bias
      if (dis(gen) < mutationRate) {
        double currentBias = neuron.get_bias();
        neuron.set_bias(currentBias + gaussian(gen));
      }
    }
  }

  // Mutate output layer
  for (auto& neuron : _network.get_output_layer()) {
    // Mutate weights
    for (size_t weightIdx = 0; weightIdx < neuron.get_input_count(); ++weightIdx) {
      if (dis(gen) < mutationRate) {
        double currentWeight = neuron.get_input_weight(weightIdx);
        neuron.set_input_weight(weightIdx, currentWeight + gaussian(gen));
      }
    }

    // Mutate bias
    if (dis(gen) < mutationRate) {
      double currentBias = neuron.get_bias();
      neuron.set_bias(currentBias + gaussian(gen));
    }
  }
}

auto Genome::randomize() -> void {
  _network.randomize();
}

auto Genome::set_layer_count(size_t count) -> void {
  _layerCount = count;
  _ready = false;
}

auto Genome::get_layer_count() const -> size_t {
  return _layerCount;
}

auto Genome::set_hidden_layer_neuron_count(size_t count) -> void {
  _neuronCount = count;
  _ready = false;
}

auto Genome::get_hidden_layer_neuron_count() const -> size_t {
  return _neuronCount;
}

auto Genome::set_input_count(size_t inputCount) -> void {
  _inputCount = inputCount;
  _ready = false;
}

auto Genome::get_input_count() const -> size_t {
  return _inputCount;
}

auto Genome::set_output_count(size_t outputCount) -> void {
  _outputCount = outputCount;
  _ready = false;
}

auto Genome::get_output_count() const -> size_t {
  return _outputCount;
}

auto Genome::validate_compatible(const Genome& other) const -> void {
  // Check if all network dimensions match
  if (_inputCount != other._inputCount) {
    throw std::invalid_argument("Input layer sizes do not match");
  }

  if (_outputCount != other._outputCount) {
    throw std::invalid_argument("Output layer sizes do not match");
  }

  if (_layerCount != other._layerCount) {
    throw std::invalid_argument("Hidden layer counts do not match");
  }

  if (_neuronCount != other._neuronCount) {
    throw std::invalid_argument("Hidden layer sizes do not match");
  }
}