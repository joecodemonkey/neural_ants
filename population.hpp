#pragma once

#include <functional>
#include <vector>

#include "ant.hpp"
#include "raylib.h"

class Population {
  const int DEFAULT_POPULATION_SIZE = 1;

 public:
  Population() = default;
  ~Population() = default;

  void set_size(int size) {
    _size = size;
  }
  [[nodiscard]] int get_size() const {
    return _size;
  }

  auto get_world_size() const -> const Vector2&;
  void set_world_size(const Vector2& world_size);

  void draw();

  void update();

  std::vector<std::reference_wrapper<Ant> > find_touching(const Vector2& position, float radius);

  /* Explicitly obstruct any copying or moving of the Population object so that the compiler
   * will warn us if we try to do so. */

  Population(const Population& other) = delete;

  Population& operator=(const Population& other) = delete;

  Population(Population&& other) = delete;

  Population& operator=(Population&& other) = delete;

  void set_texture_path(std::string const& path) {
    _texture_path = path;
  }
  [[nodiscard]] std::string const& get_texture_path() const {
    return _texture_path;
  }

 protected:
  Ant birth();

  void smite();
  void reproduce();

  std::vector<Ant> _ants;
  Vector2 _worldSize;
  int _size = DEFAULT_POPULATION_SIZE;
  std::string _texture_path;
};
