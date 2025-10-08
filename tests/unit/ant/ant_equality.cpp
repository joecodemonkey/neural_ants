#include <catch2/catch_test_macros.hpp>

#include "ant.hpp"
#include "genome.hpp"
#include "world.hpp"

TEST_CASE("Ant equality operator", "[ant]") {
  // Create a mock world for testing
  World world;

  // Helper function to create a minimal genome for testing
  auto create_minimal_genome = []() {
    Genome genome;
    // Configure a minimal neural network to avoid allocation issues
    auto& network = const_cast<NeuralNetwork&>(genome.get_network());
    network.set_input_count(2);
    network.set_hidden_layer_count(1);
    network.set_hidden_layer_neuron_count(2);
    network.set_output_neuron_count(2);
    return genome;
  };

  SECTION("Default constructed ants are equal") {
    Genome genome1 = create_minimal_genome();
    Genome genome2 = create_minimal_genome();
    Ant ant1(world, genome1);
    Ant ant2(world, genome2);

    REQUIRE(ant1 == ant2);
    REQUIRE(ant2 == ant1);
  }

  SECTION("Identical configured ants are equal") {
    Genome genome1 = create_minimal_genome();
    Genome genome2 = create_minimal_genome();
    Ant ant1(world, genome1);
    Ant ant2(world, genome2);

    // Set same values (avoiding texture-dependent operations)
    ant1.set_position({10.0f, 20.0f});
    ant1.set_energy(500.0f);
    ant1.set_dead(false);

    ant2.set_position({10.0f, 20.0f});
    ant2.set_energy(500.0f);
    ant2.set_dead(false);

    REQUIRE(ant1 == ant2);
    REQUIRE(ant2 == ant1);
  }

  SECTION("Copied ants are equal") {
    Genome genome = create_minimal_genome();
    genome.randomize();
    Ant original(world, genome);
    original.set_position({15.0f, 25.0f});
    original.set_energy(750.0f);
    original.set_dead(false);

    Ant copy(original);  // Copy constructor

    REQUIRE(original == copy);
    REQUIRE(copy == original);
  }

  SECTION("Assigned ants are equal") {
    Genome genome = create_minimal_genome();
    genome.randomize();
    Ant original(world, genome);
    original.set_position({5.0f, 15.0f});
    original.set_energy(300.0f);
    original.set_dead(false);

    Ant assigned(world, genome);
    assigned = original;  // Copy assignment

    REQUIRE(original == assigned);
    REQUIRE(assigned == original);
  }

  SECTION("Ants with different positions are not equal") {
    Genome genome = create_minimal_genome();
    Ant ant1(world, genome);
    Ant ant2(world, genome);

    ant1.set_position({10.0f, 20.0f});
    ant2.set_position({15.0f, 25.0f});

    REQUIRE_FALSE(ant1 == ant2);
    REQUIRE_FALSE(ant2 == ant1);
  }

  SECTION("Ants with different energy levels are not equal") {
    Genome genome = create_minimal_genome();
    Ant ant1(world, genome);
    Ant ant2(world, genome);

    ant1.set_energy(500.0f);
    ant2.set_energy(750.0f);

    REQUIRE_FALSE(ant1 == ant2);
    REQUIRE_FALSE(ant2 == ant1);
  }

  SECTION("Ants with different dead states are not equal") {
    Genome genome = create_minimal_genome();
    Ant ant1(world, genome);
    Ant ant2(world, genome);

    ant1.set_dead(false);
    ant2.set_dead(true);

    REQUIRE_FALSE(ant1 == ant2);
    REQUIRE_FALSE(ant2 == ant1);
  }

  SECTION("Ants with different life spans are not equal") {
    Genome genome = create_minimal_genome();
    Ant ant1(world, genome);
    Ant ant2(world, genome);

    ant1.set_life_span(ant2.get_life_span() + 1.0);

    REQUIRE_FALSE(ant1 == ant2);
  }

  SECTION("Ants with different genomes are not equal") {
    Genome genome1 = create_minimal_genome();
    Genome genome2 = create_minimal_genome();
    genome1.set_mutation_rate(0.1f);
    genome2.set_mutation_rate(0.2f);
    REQUIRE_FALSE(genome1 == genome2);

    Ant ant1(world, genome1);
    Ant ant2(world, genome2);

    REQUIRE_FALSE(ant1 == ant2);
    REQUIRE_FALSE(ant2 == ant1);
  }

  SECTION("Self equality") {
    Genome genome = create_minimal_genome();
    genome.randomize();
    Ant ant(world, genome);
    ant.set_position({10.0f, 20.0f});
    ant.set_energy(500.0f);
    ant.set_dead(false);

    REQUIRE(ant == ant);
  }
}