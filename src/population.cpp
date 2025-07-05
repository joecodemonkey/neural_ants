#include "population.hpp"

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <ant.hpp>
#include <cmath>
#include <genome.hpp>
#include <vector>
#include <world.hpp>

Population::Population(World& world) : _world(world), _size(DEFAULT_POPULATION_SIZE) {}

Population::Population(const nlohmann::json& j, World& world) : _world(world) {
  _size = j.at("size").get<int>();
  _ants.clear();
  for (const auto& ant_json : j.at("ants")) {
    auto ant = Ant(ant_json, world);
  }

  _pangenome.clear();
  for (const auto& genome_json : j.at("pangenome")) {
    _pangenome.push_back(Genome(genome_json));
  }
}

Population::Population(const Population& other)
    : _world(other._world), _ants(other._ants), _size(other._size), _pangenome(other._pangenome) {}

Population& Population::operator=(const Population& other) {
  if (this != &other) {
    _ants = other._ants;
    _size = other._size;
    _pangenome = other._pangenome;
  }
  return *this;
}

Population::Population(Population&& other)
    : _world(other._world),
      _ants(std::move(other._ants)),
      _size(other._size),
      _pangenome(std::move(other._pangenome)) {}

Population& Population::operator=(Population&& other) {
  if (this != &other) {
    _ants = std::move(other._ants);
    _size = other._size;
    _pangenome = std::move(other._pangenome);
  }
  return *this;
}

auto Population::operator==(const Population& other) const -> bool {
  return (_size == other._size && _ants == other._ants || _pangenome == other._pangenome);
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

    Genome parentA = _pangenome.back();
    _pangenome.pop_back();
    Genome parentB = _pangenome.back();
    _pangenome.pop_back();
    std::sort(_pangenome.begin(), _pangenome.end(), [](const Genome& a, const Genome& b) {
      return a.get_fitness() > b.get_fitness();
    });
    _pangenome.pop_back();  // drop 1 less fit
    _pangenome.pop_back();  // drop 1 less fit
    Genome child = parentA.breed_with(parentB);
    Ant ant(_world, child);
    ant.reset(_world.spawn_position({ant.get_bounds().width, ant.get_bounds().height}));
    return ant;
  }
  Genome genome;
  genome.set_fitness(0.0F);
  genome.randomize();
  Ant ant(_world, genome);
  ant.reset(_world.spawn_position({ant.get_bounds().width, ant.get_bounds().height}));
  return ant;
}

auto Population::update(float time) -> void {
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
