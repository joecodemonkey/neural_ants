#pragma once

#include "neural_network.hpp"
#include "raylib.h"
#include "surroundings.hpp"
#include <functional>

class Resources;
class Ant;
class World;

class Brain {
 public:
  Brain(World& world, const NeuralNetwork& neuralNetwork);
  Brain() = delete;
  Brain(const Brain& other) = default;
  auto operator=(const Brain& other) -> Brain&;

  auto update(float time, Vector2 position) -> Vector2;

 protected:
  auto update_surroundings(Vector2 position) -> void;

  std::reference_wrapper<World> _world;

  Surroundings _surroundings;
  NeuralNetwork _neuralNetwork;

  std::vector<Neuron::Value> _surroundings_encoded;

  float _last_update = 0.0F;
  const size_t TILES_SIZE = 10;
  const size_t TILES_COUNT = 10;
  const float MAX_VELOCITY = 100.0F;
  const float UPDATE_FREQUENCY = 0.1F;  // how often to consider surroundings
};
