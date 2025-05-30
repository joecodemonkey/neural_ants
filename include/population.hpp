#pragma once

#include <raylib.h>

#include <functional>
#include <string>
#include <vector>

#include "ant.hpp"

class Ant;
class World;

class Population {
 public:
  Population(World& world);
  ~Population() = default;

  auto set_size(int size) -> void;
  [[nodiscard]] auto get_size() const -> int;

  auto draw() -> void;
  auto update(float time) -> void;

  [[nodiscard]] auto get_collisions(const Vector2& position, float radius)
      -> std::vector<std::reference_wrapper<Ant>>;

  Population(const Population& other) = delete;
  Population& operator=(const Population& other) = delete;
  Population(Population&& other) = delete;
  Population& operator=(Population&& other) = delete;

  auto set_texture_path(const std::string& path) -> void;
  [[nodiscard]] auto get_texture_path() const -> const std::string&;

 protected:
  auto reproduce() -> void;

  std::vector<Ant> _ants;
  World& _world;

  const int DEFAULT_POPULATION_SIZE = 1;
  int _size;

  std::string _texturePath;
};
