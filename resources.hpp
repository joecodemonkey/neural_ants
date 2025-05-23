#pragma once

#include <raylib.h>

#include <vector>

#include "food.hpp"
#include "population.hpp"

class Resources {
 public:
  Resources() : _intended_food_count(100), _worldSize(2000.0f, 2000.0f) {}

  explicit Resources(int size, const Vector2& worldSize)
      : _intended_food_count(size), _worldSize(worldSize) {}

  ~Resources() = default;

  void set_food_count(int size) {
    _intended_food_count = size;
  }

  void update();

  void draw();

  void feed_ants(Population& population);

  void set_world_size(const Vector2&);
  [[nodiscard]] auto get_world_size() const -> const Vector2&;

 protected:
  Food new_food();

  size_t _intended_food_count = 0;
  std::vector<Food> _food;
  Vector2 _worldSize;
};
