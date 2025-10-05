#include "population.hpp"

#include <raylib.h>
#include <raymath.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

#include <algorithm>
#include <ant.hpp>
#include <cmath>
#include <genome.hpp>
#include <vector>
#include <world.hpp>

#include "pangenome.hpp"

Population::Population(World& world) : _world(world), _size(DEFAULT_POPULATION_SIZE) {}

Population::Population(const nlohmann::json& j, World& world) : _world(world) {
  _size = j.at("size").get<int>();
  _ants.clear();
  for (const auto& ant_json : j.at("ants")) {
    auto ant = Ant(ant_json, world);
    _ants.push_back(std::move(ant));
  }

  _pangenome = Pangenome(j.at("pangenome"));
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
  return (_size == other._size && _ants == other._ants && _pangenome == other._pangenome);
}

auto Population::set_size(int size) -> void {
  _size = size;
}

auto Population::get_size() const -> int {
  return _size;
}

auto Population::reproduce() -> void {
  while (_ants.size() < _size) {
    _ants.push_back(create_ant());
  }
  _ants.erase(
       std::remove_if(_ants.begin(), _ants.end(), [](const Ant& ant) {
         return ant.is_dead();
       }),
       _ants.end()
   );
}

auto Population::create_ant() -> Ant {
  if (_pangenome.size() < Pangenome::MAX_PANGENOME_SIZE) {
    // Not enough genomes for breeding - create random ant
    Genome genome;
    genome.randomize();
    genome.set_fitness(0.0F);
    Ant ant(_world, genome);
    ant.reset(_world.spawn_position({ant.get_bounds().width, ant.get_bounds().height}));
    return ant;
  }

  // Use structured breeding algorithm
  const Genome& parentA = _pangenome.sample_top_cycle();  // Top 20% round-robin
  const Genome& parentB = _pangenome.sample_random();     // Any genome
  Genome parentACopy = parentA;  // Copy for breeding (breed_with is not const)
  Genome child = parentACopy.breed_with(parentB);
  child.set_fitness(0.0F);  // Reset fitness for new ant

  Ant ant(_world, child);
  ant.reset(_world.spawn_position({ant.get_bounds().width, ant.get_bounds().height}));
  return ant;
}

auto Population::update(float time) -> void {
  tbb::parallel_for(tbb::blocked_range<size_t>(0, _ants.size()),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for (size_t i = range.begin(); i != range.end(); ++i) {
                        Ant& ant = _ants[i];

                        if (_world.out_of_bounds(ant.get_position())) {
                          ant.set_dead(true);
                        }

                        if (!ant.is_dead()) {
                          ant.update(time);
                        }
                      }
                    });

  for (Ant& ant : _ants) {
    if (ant.is_dead()) {
      // Add current life span to cumulative total
      ant.set_cumulative_life_span(ant.get_cumulative_life_span() + ant.get_life_span());

      if (ant.get_remaining_lives() > 0) {
        // Ant has remaining lives - respawn for new life
        ant.set_remaining_lives(ant.get_remaining_lives() - 1);
        ant.reset(_world.spawn_position({ant.get_bounds().width, ant.get_bounds().height}));
      } else {
        // No remaining lives - calculate mean fitness and create new ant
        double mean_life_span = ant.get_cumulative_life_span() / Ant::ANT_LIVES;
        auto genome = ant.get_genome();
        genome.set_fitness(mean_life_span);
        _fitnessData.add_data(genome.get_fitness());
        _pangenome.add(std::move(genome));
        ant = create_ant();
      }
    }
  }

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

  j["pangenome"] = _pangenome.to_json();

  return j;
}

auto Population::get_fitness_data() -> FitnessData& {
  return _fitnessData;
}

auto Population::get_fitness_data() const -> const FitnessData& {
  return _fitnessData;
}

auto Population::get_ants() -> std::vector<Ant>& {
  return _ants;
}

auto Population::set_texture_dimensions(float width, float height) -> void {
  for (auto& ant : _ants) {
    ant.set_texture_dimensions(width, height);
  }
}
