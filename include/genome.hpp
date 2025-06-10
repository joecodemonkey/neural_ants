#pragma once

#include <cmath>

#include "neural_network.hpp"
#include "neuron.hpp"
#include "random_generator.hpp"

class Genome {
 public:
  Genome() = default;
  Genome(const Genome& other);
  Genome(Genome&& other) noexcept;

  auto operator=(const Genome& other) -> Genome&;
  auto operator=(Genome&& other) noexcept -> Genome&;

  auto breed_with(const Genome& other) -> Genome;
  auto get_network() const -> const NeuralNetwork&;

  auto mutate() -> void;
  auto randomize() -> void;

  auto set_mutation_rate(double mutationRate) -> void;
  auto get_mutation_rate() const -> double;
  auto get_fitness() const -> double;
  auto set_fitness(double fitness) -> void;

 protected:
  auto breed_network(NeuralNetwork& childNetwork) -> void;
  auto breed_layer(const NeuralNetwork::Layer& parent, NeuralNetwork::Layer& childLayer) -> void;
  auto breed_neuron(const Neuron& parentNeuron, Neuron& childNeuron) -> void;

  auto mutate_layer(NeuralNetwork::Layer& layer);
  auto mutate_neuron(Neuron& neuron);
  auto should_mutate() -> bool;
  auto mutation_amount() -> double;

  NeuralNetwork _network;
  double _mutationRate = 0.1F;
  double _fitness = 0.0F;
  RandomGenerator _rng;
};
