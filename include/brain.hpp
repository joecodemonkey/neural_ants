#include "genome.hpp"
#include "neural_network.hpp"
#include "raylib.h"
#include "surroundings.hpp"

#pragma once

class Resources;
class Ant;
class World;

class Brain {
 public:
  Brain(World& world, const NeuralNetwork& neuralNetwork);
  Brain() = delete;
  Brain(const Brain& other) = default;
  auto operator=(const Brain& other) -> Brain& = default;

  auto update(float time, Vector2 position) -> Vector2;

 protected:
  auto update_surroundings(Surroundings& surr, const size_t tile_size, Vector2 position) -> void;

  World& _world;

  Surroundings _near;
  Surroundings _far;
  NeuralNetwork _neuralNetwork;

  std::vector<Neuron::Value> _surroundings_encoded;

  float _last_update = 0.0F;
  const size_t NEAR_TILES_SIZE = 1;
  const size_t NEAR_TILES_COUNT = 5;
  const size_t FAR_TILES_SIZE = 10;
  const size_t FAR_TILES_COUNT = 5;
  const float MAX_VELOCITY = 100.0F;
  const float UPDATE_FREQUENCY = 0.1F;  // how often to consider surroundings
};