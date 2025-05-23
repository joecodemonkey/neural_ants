#include "world.hpp"

#include <iostream>

#include "population.hpp"
#include "raylib.h"
#include "resources.hpp"

World::World() : _bounds(World::DEFAULT_BOUNDS) {
  _population.set_world_size({World::DEFAULT_BOUNDS.width, World::DEFAULT_BOUNDS.height});
}

auto World::get_bounds() const -> const Rectangle& {
  return _bounds;
}

auto World::get_population() const -> const Population& {
  return _population;
}

auto World::get_population() -> Population& {
  return _population;
}

auto World::get_resources() -> Resources& {
  return _resources;
}

auto World::get_resources() const -> const Resources& {
  return _resources;
}

void World::update(float time) {
  _resources.update();
  _resources.feed_ants(_population);
  _population.update();
}

void World::draw() {
  DrawRectangle(_bounds.x, _bounds.y, _bounds.width, _bounds.height, WHITE);
  _resources.draw();
  _population.draw();
}