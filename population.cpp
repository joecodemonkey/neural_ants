#include "population.hpp"

#include <raymath.h>

#include <algorithm>
#include <cmath>  // Include this for std::ceil

#include "ant.hpp"
#include "raylib.h"
#include "world.hpp"

Population::Population(World& world) : _world(world), _size(DEFAULT_POPULATION_SIZE) {}

auto Population::set_size(int size) -> void {
  _size = size;
}

auto Population::get_size() const -> int {
  return _size;
}

auto Population::draw() -> void {
  for (auto& ant : _ants) {
    ant.draw();
  }
}

auto Population::birth() -> Ant {
  Ant ant;
  if (!_texturePath.empty()) {
    ant.set_ant_texture_path(_texturePath);
  }
  ant.reset(_world.spawn_position(ant.get_size()));

  return ant;
}

auto Population::reproduce() -> void {
  // ensure the population vector matches the size of the population
  while (_ants.size() < _size) {
    _ants.push_back(birth());
  }

  // if the population vector is larger than the size of the population, move live ants at the end
  // over the dead ones
  while (_ants.size() > _size) {
    // stupidly inefficient, but it works and we don't care about performance right now
    auto it =
        std::remove_if(_ants.begin(), _ants.end(), [](const Ant& ant) { return ant.is_dead(); });
    _ants.erase(it, _ants.end());
  }

  // rebirth dead ants
  for (auto& ant : _ants) {
    if (ant.is_dead()) {
      ant.reset(_world.spawn_position(ant.get_size()));
    }
  }
}

auto Population::update() -> void {
  // update all ants
  for (Ant& ant : _ants) {
    ant.update();
  }

  // ensure population size is maintained
  reproduce();
}

auto Population::find_touching(const Vector2& position, float radius)
    -> std::vector<std::reference_wrapper<Ant>> {
  std::vector<std::reference_wrapper<Ant>> touchingAnts;
  for (Ant& ant : _ants) {
    const float antRadius = ant.get_size().x / 2.0f;
    const float distance = Vector2Distance(position, ant.get_position());
    if (distance < (radius + antRadius)) {
      touchingAnts.push_back(std::ref(ant));
    }
  }
  return touchingAnts;
}

auto Population::set_texture_path(const std::string& path) -> void {
  _texturePath = path;
}

auto Population::get_texture_path() const -> const std::string& {
  return _texturePath;
}
