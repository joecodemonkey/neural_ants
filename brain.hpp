#include "genome.hpp"
#include "raylib.h"
#include "surroundings.hpp"

#pragma once

class Resources;
class Ant;
class World;

class Brain {
 public:
  Brain(Ant& ant, World& world);
  Brain() = delete;

  auto update(float time) -> void;

  auto set_bounds(Rectangle& bounds) -> void;
  [[nodiscard]] auto get_bounds() -> Rectangle&;
  [[nodiscard]] auto get_bounds() const -> const Rectangle&;

  [[nodiscard]] auto get_genome() -> Genome&;
  [[nodiscard]] auto get_genome() const -> const Genome&;
  auto set_genome(const Genome& genome) -> void;

 protected:
  auto update_surroundings(Surroundings& surr, const size_t tile_size) -> void;

  Genome _genome;

  Ant& _ant;
  World& _world;

  Surroundings _near;
  Surroundings _far;

  std::vector<Neuron::Value> _surroundings_encoded;

  float _last_update = 0.0F;
  const size_t NEAR_TILES_SIZE = 1;
  const size_t NEAR_TILES_COUNT = 5;
  const size_t FAR_TILES_SIZE = 10;
  const size_t FAR_TILES_COUNT = 5;
  const float MAX_VELOCITY = 100.0F;
  const float UPDATE_FREQUENCY = 0.1F;  // how often to consider surroundings
};