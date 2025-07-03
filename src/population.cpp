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

Population::Population(const nlohmann::json& j, World& world) : _world(world) {
  _size = j.at("size").get<int>();
  // Note: texture path and loaded state remain unchanged from game initialization

  _ants.clear();
  for (const auto& ant_json : j.at("ants")) {
    Ant ant(ant_json, world);
    // Apply texture immediately if already loaded, otherwise will be applied in update()
    if (_textureLoaded && !_texturePath.empty()) {
      ant.set_texture(_texture);
    }
    _ants.push_back(ant);
  }

  _pangenome.clear();
  for (const auto& genome_json : j.at("pangenome")) {
    _pangenome.push_back(Genome(genome_json));
  }
}

Population::Population(const Population& other)
    : _world(other._world),
      _ants(other._ants),
      _size(other._size),
      _textureLoaded(other._textureLoaded),
      _texturePath(other._texturePath),
      _pangenome(other._pangenome),
      _texture(other._texture) {}

Population& Population::operator=(const Population& other) {
  if (this != &other) {
    _ants = other._ants;
    _size = other._size;
    _textureLoaded = other._textureLoaded;
    _texturePath = other._texturePath;
    _pangenome = other._pangenome;
    _texture = other._texture;
  }
  return *this;
}

Population::Population(Population&& other)
    : _world(other._world),
      _ants(std::move(other._ants)),
      _size(other._size),
      _textureLoaded(other._textureLoaded),
      _texturePath(std::move(other._texturePath)),
      _pangenome(std::move(other._pangenome)),
      _texture(other._texture) {
  other._size = 0;
  other._textureLoaded = false;
  other._texture = {};
}

Population& Population::operator=(Population&& other) {
  if (this != &other) {
    _ants = std::move(other._ants);
    _size = other._size;
    _textureLoaded = other._textureLoaded;
    _texturePath = std::move(other._texturePath);
    _pangenome = std::move(other._pangenome);
    _texture = other._texture;

    other._size = 0;
    other._textureLoaded = false;
    other._texture = {};
  }
  return *this;
}

auto Population::operator==(const Population& other) const -> bool {
  // Compare all relevant member variables
  if (_textureLoaded != other._textureLoaded || _texturePath != other._texturePath ||
      _size != other._size) {
    return false;
  }
  if (_ants != other._ants || _pangenome != other._pangenome) {
    return false;
  }

  return true;
}

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

    // Apply texture to all existing ants (important for loaded games)
    for (Ant& ant : _ants) {
      ant.set_texture(_texture);
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
      _fitnessData.add_data(genome.get_fitness());
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

auto Population::to_json() const -> nlohmann::json {
  nlohmann::json j;
  j["size"] = _size;

  nlohmann::json ants_array = nlohmann::json::array();
  for (const auto& ant : _ants) {
    ants_array.push_back(ant.to_json());
  }
  j["ants"] = ants_array;

  nlohmann::json pangenome_array = nlohmann::json::array();
  for (const auto& genome : _pangenome) {
    pangenome_array.push_back(genome.to_json());
  }
  j["pangenome"] = pangenome_array;

  return j;
}

auto Population::get_fitness_data() -> FitnessData& {
  return _fitnessData;
}

auto Population::get_fitness_data() const -> const FitnessData& {
  return _fitnessData;
}
