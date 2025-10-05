#pragma once

#include <raylib.h>

#include <ant.hpp>
#include <containers/circular_stats.hpp>
#include <functional>
#include <genome.hpp>
#include <nlohmann/json.hpp>
#include <vector>

#include "pangenome.hpp"

// Forward declaration
class World;

class Population {
 public:
  typedef Containers::CircularStats<double> FitnessData;
  Population(World& world);
  Population(const nlohmann::json& j, World& world);
  ~Population() = default;

  auto set_size(int size) -> void;
  [[nodiscard]] auto get_size() const -> int;

  auto update(float time) -> void;

  [[nodiscard]] auto get_collisions(const Vector2& position, float radius)
      -> std::vector<std::reference_wrapper<Ant>>;

  Population(const Population& other);
  Population& operator=(const Population& other);
  Population(Population&& other);
  Population& operator=(Population&& other);
  auto operator==(const Population& other) const -> bool;

  auto to_json() const -> nlohmann::json;

  auto get_fitness_data() -> FitnessData&;
  auto get_fitness_data() const -> const FitnessData&;

  auto get_ants() -> std::vector<Ant>&;
  auto set_texture_dimensions(float width, float height) -> void;

 protected:
  auto reproduce() -> void;
  auto create_ant() -> Ant;

  std::vector<Ant> _ants;
  World& _world;

  const int DEFAULT_POPULATION_SIZE = 1;
  int _size;

  Pangenome _pangenome;

  FitnessData _fitnessData;
};
