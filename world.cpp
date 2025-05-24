#include "world.hpp"

#include <iostream>

#include "population.hpp"
#include "raylib.h"
#include "resources.hpp"

World::World() : _resources(*this) {
  _bounds = World::DEFAULT_BOUNDS;
  _spawnMargin = DEFAULT_SPAWN_MARGIN;
  _population.set_world_size({_bounds.width, _bounds.height});
  update_spawn_rect();
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

auto World::get_spawn_bounds() const -> const Rectangle& {
  return _spawnBounds;
}

auto World::set_spawn_margin(float margin) -> void {
  _spawnMargin = margin;
  update_spawn_rect();
}

auto World::get_spawn_margin() const -> float {
  return _spawnMargin;
}

auto World::update_spawn_rect() -> void {
  float margin = _bounds.width * _spawnMargin;
  _spawnBounds = {_bounds.x + margin,
                  _bounds.y + margin,
                  _bounds.width - (2 * margin),
                  _bounds.height - (2 * margin)};
}

auto World::update(float time) -> void {
  _resources.update(time);
  _resources.feed_ants(_population);
  _population.update();
}

auto World::draw() -> void {
  DrawRectangle(_bounds.x, _bounds.y, _bounds.width, _bounds.height, WHITE);
  _resources.draw();
  _population.draw();
}

auto World::out_of_bounds(const Rectangle& object) const -> bool {
  return object.x < _bounds.x || object.y < _bounds.y ||
         object.x + object.width > _bounds.x + _bounds.width ||
         object.y + object.height > _bounds.y + _bounds.height;
}
