#include "population.hpp"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <cmath>  // Include this for std::ceil
#include <iostream>
#include <vector>

#include "ant.hpp"
#include "genome.hpp"
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
    _ants.push_back(create_ant());
  }

  while (_ants.size() > _size) {
    _ants.erase(
        std::remove_if(_ants.begin(), _ants.end(), [](const Ant& ant) { return ant.is_dead(); }));
  }
}

auto Population::create_ant() -> Ant {
  if (_pangenome.size() > TARGET_PANGENOME_SIZE) {
    std::sort(_pangenome.begin(), _pangenome.end(), [](const Genome& a, const Genome& b) {
      return a.get_fitness() > b.get_fitness();
    });

    // print mean genome fitness
    double totalFitness = 0.0;
    for (const auto& genome : _pangenome) {
      totalFitness += genome.get_fitness();
    }
    double meanFitness = totalFitness / _pangenome.size();
    std::cerr << "Mean genome fitness: " << meanFitness << std::endl;
    Genome parentA = _pangenome.back();  // take 1 fit
    _pangenome.pop_back();
    Genome parentB = _pangenome.back();  // take 1 fit
    _pangenome.pop_back();
    std::sort(_pangenome.begin(), _pangenome.end(), [](const Genome& a, const Genome& b) {
      return a.get_fitness() > b.get_fitness();
    });
    _pangenome.pop_back();  // drop 1 less fit
    _pangenome.pop_back();  // drop 1 less fit
    Genome child = parentA.breed_with(parentB);
    Ant ant(_world, child);
    if (!_texturePath.empty()) {
      ant.set_texture(_texture);
    }
    ant.reset(_world.spawn_position({ant.get_bounds().width, ant.get_bounds().height}));
    return ant;
  }
  Genome genome;
  genome.set_fitness(0.0F);
  genome.randomize();
  Ant ant(_world, genome);
  if (!_texturePath.empty()) {
    ant.set_texture(_texture);
  }
  ant.reset(_world.spawn_position({ant.get_bounds().width, ant.get_bounds().height}));
  return ant;
}

auto Population::update(float time) -> void {
  if (_textureLoaded == false && !_texturePath.empty()) {
    _texture = LoadTexture(_texturePath.c_str());
    _textureLoaded = true;

    if (!IsTextureValid(_texture)) {
      throw std::runtime_error("Failed to load ant texture at path: " + _texturePath);
    }
  }
  // update all ants
  for (Ant& ant : _ants) {
    if (_world.out_of_bounds(ant.get_position())) {
      ant.set_dead(true);
    }

    if (ant.is_dead()) {
      auto genome = ant.get_genome();
      genome.set_fitness(ant.get_life_span());
      _pangenome.push_back(genome);
      ant = create_ant();
    } else {
      ant.update(time);
    }
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
