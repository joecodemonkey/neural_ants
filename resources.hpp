#pragma once

class World;
class Population;

#include <vector>

#include "food.hpp"
#include "raylib.h"

class Resources {
 public:
  Resources() = delete;

  Resources(World& world);

  ~Resources() = default;

  auto get_food_count() const -> int;
  auto set_food_count(int size) -> void;

  auto update(float time) -> void;

  auto draw() const -> void;

  auto feed_ants(Population& population) -> void;
  auto food_in_rect(const Rectangle rect) const -> bool;

 protected:
  auto new_food() -> Food;

  World& _world;
  size_t _food_count;
  std::vector<Food> _food;

  const size_t DEFAULT_COUNT = 100;
  const float BOUNDS_PADDING = 0.20f;
};
