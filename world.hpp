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

  void update(float time);
  void draw();

 protected:
  const Rectangle DEFAULT_BOUNDS = {0.0f, 0.0f, 1000.0f, 1000.0f};

  Resources _resources;
  Population _population;
  Rectangle _bounds;
};