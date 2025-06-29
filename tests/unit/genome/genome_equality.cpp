#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "genome.hpp"
#include "neural_network.hpp"

using Catch::Approx;

TEST_CASE("Genome equality", "[genome]") {
  SECTION("Identical genomes are equal") {
    Genome genome1;
    Genome genome2;

    // Set same values
    genome1.set_mutation_rate(0.5F);
    genome1.set_fitness(100.0F);
    genome2.set_mutation_rate(0.5F);
    genome2.set_fitness(100.0F);

    REQUIRE(genome1 == genome2);
    REQUIRE(genome2 == genome1);
  }

  SECTION("Different mutation rates make genomes unequal") {
    Genome genome1;
    Genome genome2;

    // Set same fitness but different mutation rates
    genome1.set_mutation_rate(0.5F);
    genome1.set_fitness(100.0F);
    genome2.set_mutation_rate(0.3F);
    genome2.set_fitness(100.0F);

    REQUIRE_FALSE(genome1 == genome2);
    REQUIRE_FALSE(genome2 == genome1);
  }

  SECTION("Different fitness values make genomes unequal") {
    Genome genome1;
    Genome genome2;

    // Set same mutation rate but different fitness
    genome1.set_mutation_rate(0.5F);
    genome1.set_fitness(100.0F);
    genome2.set_mutation_rate(0.5F);
    genome2.set_fitness(200.0F);

    REQUIRE_FALSE(genome1 == genome2);
    REQUIRE_FALSE(genome2 == genome1);
  }

  SECTION("Different neural networks make genomes unequal") {
    Genome genome1;
    Genome genome2;

    // Set same mutation rate and fitness
    genome1.set_mutation_rate(0.5F);
    genome1.set_fitness(100.0F);
    genome2.set_mutation_rate(0.5F);
    genome2.set_fitness(100.0F);

    // Randomize one network to make them different
    genome1.randomize();

    REQUIRE_FALSE(genome1 == genome2);
    REQUIRE_FALSE(genome2 == genome1);
  }

  SECTION("Copy constructor creates equal genomes") {
    Genome original;
    original.set_mutation_rate(0.5F);
    original.set_fitness(100.0F);
    original.randomize();

    Genome copy(original);

    REQUIRE(original == copy);
    REQUIRE(copy == original);
  }

  SECTION("Assignment operator creates equal genomes") {
    Genome original;
    original.set_mutation_rate(0.5F);
    original.set_fitness(100.0F);
    original.randomize();

    Genome assigned;
    assigned = original;

    REQUIRE(original == assigned);
    REQUIRE(assigned == original);
  }

  SECTION("Move constructor preserves equality") {
    Genome original;
    original.set_mutation_rate(0.5F);
    original.set_fitness(100.0F);
    original.randomize();

    Genome moved(std::move(original));

    // Note: After move, original is in undefined state, so we can't compare
    // But we can verify the moved genome has the expected values
    REQUIRE(moved.get_mutation_rate() == 0.5F);
    REQUIRE(moved.get_fitness() == 100.0F);
  }

  SECTION("Self-comparison is always true") {
    Genome genome;
    genome.set_mutation_rate(0.5F);
    genome.set_fitness(100.0F);
    genome.randomize();

    REQUIRE(genome == genome);
  }

  SECTION("Default constructed genomes are equal") {
    Genome genome1;
    Genome genome2;

    REQUIRE(genome1 == genome2);
    REQUIRE(genome2 == genome1);
  }

  SECTION("Genomes with different network structures are unequal") {
    // Create genomes with different network configurations
    Genome genome1;
    Genome genome2;

    // Set same basic properties
    genome1.set_mutation_rate(0.5F);
    genome1.set_fitness(100.0F);
    genome2.set_mutation_rate(0.5F);
    genome2.set_fitness(100.0F);

    // Randomize both to ensure different internal states
    genome1.randomize();
    genome2.randomize();

    // They should be different due to randomization
    REQUIRE_FALSE(genome1 == genome2);
    REQUIRE_FALSE(genome2 == genome1);
  }
}