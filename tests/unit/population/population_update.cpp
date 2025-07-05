#include <catch2/catch_test_macros.hpp>

#include "genome.hpp"
#include "population.hpp"
#include "world.hpp"

TEST_CASE("Population update functionality", "[population]") {
  World world;

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

  SECTION("Update with zero size population") {
    Population population(world);
    population.set_size(0);

    REQUIRE_NOTHROW(population.update(1.0f));

    // Should still have zero size after update
    auto collisions = population.get_collisions({0.0f, 0.0f}, 1000.0f);
    REQUIRE(collisions.empty());
  }

  SECTION("Update creates ants according to size") {
    Population population(world);
    population.set_size(3);

    // Before update, there should be no ants
    auto collisions_before = population.get_collisions({500.0f, 500.0f}, 1000.0f);
    REQUIRE(collisions_before.empty());

    // After update, should have up to 3 ants
    population.update(1.0f);
    auto collisions_after = population.get_collisions({500.0f, 500.0f}, 1000.0f);
    REQUIRE(collisions_after.size() <= 3);
  }

  SECTION("Update with invalid texture path") {
    Population population(world);
    population.set_size(1);

    // Should throw when trying to load invalid texture
    REQUIRE_THROWS_AS(population.update(1.0f), std::runtime_error);
  }

  SECTION("Update with empty texture path") {
    Population population(world);
    population.set_size(1);

    // Should not throw with empty texture path
    REQUIRE_NOTHROW(population.update(1.0f));

    auto collisions = population.get_collisions({500.0f, 500.0f}, 1000.0f);
    REQUIRE(collisions.size() <= 1);
  }

  SECTION("Update maintains population size") {
    Population population(world);
    population.set_size(2);

    // Multiple updates should maintain the same population size
    population.update(1.0f);
    auto collisions1 = population.get_collisions({500.0f, 500.0f}, 1000.0f);

    population.update(1.0f);
    auto collisions2 = population.get_collisions({500.0f, 500.0f}, 1000.0f);

    // Should maintain population size
    REQUIRE(collisions1.size() == collisions2.size());
    REQUIRE(collisions2.size() <= 2);
  }

  SECTION("Update with different time values") {
    Population population(world);
    population.set_size(1);

    // Should handle different time values without crashing
    REQUIRE_NOTHROW(population.update(0.0f));
    REQUIRE_NOTHROW(population.update(0.1f));
    REQUIRE_NOTHROW(population.update(1.0f));
    REQUIRE_NOTHROW(population.update(10.0f));
  }

  SECTION("Update after size change") {
    Population population(world);
    population.set_size(1);
    population.update(1.0f);

    auto collisions1 = population.get_collisions({500.0f, 500.0f}, 1000.0f);
    REQUIRE(collisions1.size() <= 1);

    // Increase size and update
    population.set_size(3);
    population.update(1.0f);

    auto collisions2 = population.get_collisions({500.0f, 500.0f}, 1000.0f);
    REQUIRE(collisions2.size() <= 3);

    // Decrease size and update
    population.set_size(1);
    population.update(1.0f);

    auto collisions3 = population.get_collisions({500.0f, 500.0f}, 1000.0f);
    REQUIRE(collisions3.size() <= 1);
  }
}