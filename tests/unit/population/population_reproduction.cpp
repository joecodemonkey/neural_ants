#include <catch2/catch_test_macros.hpp>

#include "genome.hpp"
#include "pangenome.hpp"
#include "population.hpp"
#include "texture_cache.hpp"
#include "world.hpp"

// Test class to access protected members
class PopulationTestAccess : public Population {
 public:
  PopulationTestAccess(World& world) : Population(world) {}

  using Population::_ants;
  using Population::_pangenome;
  using Population::create_ant;
  using Population::reproduce;
};

TEST_CASE("Population reproduction logic", "[population]") {
  TextureCache textureCache;
  World world(textureCache);

  // Helper function to create a minimal genome for testing
  auto create_minimal_genome = []() {
    Genome genome;
    auto& network = const_cast<NeuralNetwork&>(genome.get_network());
    network.set_input_count(2);
    network.set_hidden_layer_count(1);
    network.set_hidden_layer_neuron_count(2);
    network.set_output_neuron_count(2);
    return genome;
  };

  SECTION("Reproduce adds ants when under size") {
    PopulationTestAccess population(world);
    population.set_size(3);

    REQUIRE(population._ants.size() == 0);

    population.reproduce();

    REQUIRE(population._ants.size() == 3);
  }

  SECTION("Reproduce maintains size when at target") {
    PopulationTestAccess population(world);
    population.set_size(2);

    // Manually add ants to reach target size
    population._ants.push_back(population.create_ant());
    population._ants.push_back(population.create_ant());

    REQUIRE(population._ants.size() == 2);

    population.reproduce();

    REQUIRE(population._ants.size() == 2);
  }

  SECTION("Create ant generates valid ant") {
    PopulationTestAccess population(world);

    Ant ant = population.create_ant();

    // Verify ant is in valid state
    REQUIRE_FALSE(ant.is_dead());
    REQUIRE(ant.get_energy() > 0.0f);
    REQUIRE(ant.get_radius() >= 0.0f);
  }

  SECTION("Create ant with empty pangenome uses random genome") {
    PopulationTestAccess population(world);

    REQUIRE(population._pangenome.empty());

    Ant ant1 = population.create_ant();
    Ant ant2 = population.create_ant();

    // Both ants should be created successfully
    REQUIRE_FALSE(ant1.is_dead());
    REQUIRE_FALSE(ant2.is_dead());
  }

  SECTION("Create ant with small pangenome uses random genome") {
    PopulationTestAccess population(world);

    // Add some genomes but stay under MAX_PANGENOME_SIZE
    for (int i = 0; i < 10; ++i) {
      Genome genome = create_minimal_genome();
      genome.set_fitness(static_cast<double>(i));
      population._pangenome.add(std::move(genome));
    }

    REQUIRE(population._pangenome.size() < Pangenome::MAX_PANGENOME_SIZE);

    Ant ant = population.create_ant();

    REQUIRE_FALSE(ant.is_dead());
  }

  SECTION("Create ant with large pangenome uses breeding") {
    PopulationTestAccess population(world);

    // Fill pangenome to MAX_PANGENOME_SIZE to trigger breeding
    for (size_t i = 0; i < Pangenome::MAX_PANGENOME_SIZE; ++i) {
      Genome genome = create_minimal_genome();
      genome.set_fitness(static_cast<double>(i));
      population._pangenome.add(std::move(genome));
    }

    REQUIRE(population._pangenome.size() == Pangenome::MAX_PANGENOME_SIZE);

    Ant ant = population.create_ant();

    // The pangenome should still be at MAX_PANGENOME_SIZE (it doesn't shrink during breeding)
    REQUIRE(population._pangenome.size() == Pangenome::MAX_PANGENOME_SIZE);
    REQUIRE_FALSE(ant.is_dead());
  }

  SECTION("Breeding selects fittest genomes as parents") {
    PopulationTestAccess population(world);

    // Create genomes with known fitness values
    std::vector<double> fitness_values = {1.0, 5.0, 3.0, 8.0, 2.0};
    for (double fitness : fitness_values) {
      Genome genome = create_minimal_genome();
      genome.set_fitness(fitness);
      population._pangenome.add(std::move(genome));
    }

    // Add more genomes to reach threshold
    for (size_t i = fitness_values.size(); i < Pangenome::MAX_PANGENOME_SIZE; ++i) {
      Genome genome = create_minimal_genome();
      genome.set_fitness(0.1);  // Low fitness
      population._pangenome.add(std::move(genome));
    }

    REQUIRE(population._pangenome.size() == Pangenome::MAX_PANGENOME_SIZE);

    Ant ant = population.create_ant();

    REQUIRE_FALSE(ant.is_dead());
    REQUIRE(population._pangenome.size() == Pangenome::MAX_PANGENOME_SIZE);
  }
}