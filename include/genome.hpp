#pragma once

#include <cmath>

#include "neural_network.hpp"
#include "random_generator.hpp"

class Genome {
 public:
  // Default constructor
  Genome();

  // Copy constructor
  Genome(const Genome& other);

  // Move constructor
  Genome(Genome&& other) noexcept;

  // Copy assignment
  auto operator=(const Genome& other) -> Genome&;

  // Move assignment
  auto operator=(Genome&& other) noexcept -> Genome&;

  auto breed_with(const Genome& other) -> Genome;

  auto get_network() const -> const NeuralNetwork&;

  auto mutate() -> void;
  auto randomize() -> void;

  auto set_mutation_rate(double mutationRate) -> void;
  auto get_mutation_rate() const -> double;

 protected:
  auto breed_layer(const NeuralNetwork::Layer& parent1,
                   const NeuralNetwork::Layer& parent2,
                   NeuralNetwork::Layer& child) -> void;

  auto mutate_layer(NeuralNetwork::Layer& layer);
  auto mutate_neuron(Neuron& neuron);
  auto should_mutate() -> bool;
  auto mutation_amount() -> double;

  NeuralNetwork _network;
  double _mutationRate = 0.1F;
  RandomGenerator _rng;
};
