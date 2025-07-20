#pragma once

#include <cmath>
#include <nlohmann/json.hpp>

#include "neural_network.hpp"
#include "neuron.hpp"
#include "random_generator.hpp"

class Genome {
 public:
  Genome() = default;
  Genome(const Genome& other);
  Genome(Genome&& other) noexcept;
  Genome(const nlohmann::json& json);

  auto operator=(const Genome& other) -> Genome&;
  auto operator=(Genome&& other) noexcept -> Genome&;
  auto operator==(const Genome& other) const -> bool;

  auto breed_with(const Genome& other) -> Genome;
  auto get_network() const -> const NeuralNetwork&;

  auto mutate() -> void;
  auto randomize() -> void;

  auto set_mutation_rate(double mutationRate) -> void;
  auto get_mutation_rate() const -> double;
  auto get_fitness() const -> double;
  auto set_fitness(double fitness) -> void;

  auto get_children_count() const -> size_t;
  auto set_children_count(size_t count) -> void;
  auto increment_children_count() -> void;

  auto to_json() const -> nlohmann::json;

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
  size_t _childrenCount = 0;
  RandomGenerator _rng;
};
