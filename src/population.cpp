#include "population.hpp"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <cmath>  // Include this for std::ceil

#include "ant.hpp"
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

auto Population::reproduce() -> void {
  // ensure the population vector matches the size of the population
  while (_ants.size() < _size) {
    Ant ant(_world);
    if (!_texturePath.empty()) {
      ant.set_texture_path(_texturePath);
    }
    ant.reset(_world.spawn_position({ant.get_bounds().width, ant.get_bounds().height}));

    _ants.push_back(ant);
  }

  while (_ants.size() > _size) {
    _ants.erase(
        std::remove_if(_ants.begin(), _ants.end(), [](const Ant& ant) { return ant.is_dead(); }));
  }
}

auto Population::update(float time) -> void {
  // update all ants
  for (Ant& ant : _ants) {
    if (_world.out_of_bounds(ant.get_position())) {
      ant.set_dead(true);
    }

    if (ant.is_dead()) {
      ant.reset(_world.spawn_position({ant.get_bounds().width, ant.get_bounds().height}));
    }
    ant.update(time);
  }

  // ensure population size is maintained
  reproduce();
}

auto Population::get_collisions(const Vector2& position, float radius)
    -> std::vector<std::reference_wrapper<Ant>> {
  std::vector<std::reference_wrapper<Ant>> touchingAnts;
  for (Ant& ant : _ants) {
    if (ant.collides(position, radius)) {
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
