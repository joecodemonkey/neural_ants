
#include <raylib.h>

#include "genome.hpp"

class Population;
class Resources;
class Surroundings;
class Genome;

#pragma once

class Brain {
  Brain();

  void update(const Population& population, const Resources& resources, const Vector2& position);

  float get_speed() const;
  float get_direction() const;

  void set_bounds(Rectangle& bounds);
  Rectangle& get_bounds();
  const Rectangle& get_bounds() const;

  Genome& get_genome();
  const Genome& get_genome() const;
  void set_genome(const Genome& genome);

  void set_far_tile_size(size_t size);
  size_t get_far_tile_size() const;
  void set_far_tile_window_size(size_t size);
  size_t get_far_tile_window_size() const;

 protected:
  float _direction = 0.0f;
  float _speed = 0.0f;

  Genome _genome;
  Rectangle _bounds;

  Surroundings _near_eye;
  Surroundings _far_eye;
};