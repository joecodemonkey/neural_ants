#pragma once

#include "population.hpp"
#include "raylib.h"
#include "resources.hpp"

class World {
 public:
  World();

  [[nodiscard]] auto get_population() const -> const Population&;
  [[nodiscard]] auto get_population() -> Population&;

  [[nodiscard]] auto get_resources() -> Resources&;
  [[nodiscard]] auto get_resources() const -> const Resources&;

  [[nodiscard]] auto get_bounds() const -> const Rectangle&;

  [[nodiscard]] auto get_spawn_bounds() const -> const Rectangle&;
  auto set_spawn_margin(float) -> void;
  [[nodiscard]] auto get_spawn_margin() const -> float;

  auto update(float time) -> void;
  auto draw() -> void;

  [[nodiscard]] auto out_of_bounds(const Rectangle& object) const -> bool;

 protected:
  const Rectangle DEFAULT_BOUNDS = {0.0f, 0.0f, 1000.0f, 1000.0f};
  const float DEFAULT_SPAWN_MARGIN = 0.20F;

  auto update_spawn_rect() -> void;

  Resources _resources;
  Population _population;
  Rectangle _bounds;
  Rectangle _spawnBounds;
  float _spawnMargin;
};