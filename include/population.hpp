#pragma once

#include <raylib.h>

#include <ant.hpp>
#include <containers/circular_stats.hpp>
#include <functional>
#include <genome.hpp>
#include <nlohmann/json.hpp>
#include <vector>

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

  auto draw() -> void;
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

 protected:
  auto reproduce() -> void;
  auto create_ant() -> Ant;

  std::vector<Ant> _ants;
  World& _world;

  const int DEFAULT_POPULATION_SIZE = 1;
  int _size;

  static constexpr size_t TARGET_PANGENOME_SIZE = 100;

  std::vector<Genome> _pangenome;

  FitnessData _fitnessData;
};
