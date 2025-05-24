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

  void set_food_count(int size) {
    _intended_food_count = size;
  }

  void update(float time);

  void draw() const;

  void feed_ants(Population& population);
  bool food_in_rect(const Rectangle rect) const;

 protected:
  Food new_food();

  World& _world;
  size_t _intended_food_count = 0;
  std::vector<Food> _food;

  const size_t DEFAULT_COUNT = 100;
  const float BOUNDS_PADDING = 0.20f;
};
