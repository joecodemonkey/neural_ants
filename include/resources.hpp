#pragma once

class World;
class Population;

#include <nlohmann/json.hpp>
#include <vector>

#include "food.hpp"
#include "raylib.h"

class Resources {
 public:
  Resources() = delete;

  Resources(World& world);
  Resources(const nlohmann::json& j, World& world);
  Resources(const Resources& other);
  Resources& operator=(const Resources& other);
  bool operator==(const Resources& other) const;

  ~Resources() = default;

  auto get_food_count() const -> int;
  auto set_food_count(int size) -> void;

  auto update(float time) -> void;

  auto draw() const -> void;

  auto feed_ants(Population& population) -> void;
  auto food_in_rect(const Rectangle& rect) const -> bool;

  auto to_json() const -> nlohmann::json;

 protected:
  auto food_position() -> Vector2;

  World& _world;
  size_t _food_count;
  std::vector<Food> _food;

  const size_t DEFAULT_COUNT = 200;
};
