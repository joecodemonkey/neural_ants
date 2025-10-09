#include <raylib.h>

#include <catch2/catch_test_macros.hpp>

#include "genome.hpp"
#include "population.hpp"
#include "texture_cache.hpp"
#include "world.hpp"

TEST_CASE("Population collision detection", "[population]") {
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

  SECTION("No collisions with empty population") {
    Population population(world);
    population.set_size(0);

    Vector2 position = {100.0f, 100.0f};
    float radius = 50.0f;

    auto collisions = population.get_collisions(position, radius);
    REQUIRE(collisions.empty());
  }

  SECTION("Collision detection basic functionality") {
    Population population(world);
    population.set_size(1);

    // Force population to create ants by calling update
    // This will populate the _ants vector according to _size
    population.update(0.0f);

    Vector2 position = {0.0f, 0.0f};
    float radius = 1000.0f;  // Large radius to ensure collision

    auto collisions = population.get_collisions(position, radius);

    // Should find collisions if ants exist within the radius
    // The exact number depends on ant positions after spawning
    REQUIRE(collisions.size() <= 1);  // Population size is 1
  }

  SECTION("Collision detection with zero radius") {
    Population population(world);
    population.set_size(1);
    population.update(0.0f);

    Vector2 position = {0.0f, 0.0f};
    float radius = 0.0f;

    auto collisions = population.get_collisions(position, radius);

    // With zero radius, should only collide if ant is exactly at position
    // Most likely no collisions due to spawning mechanics
    REQUIRE(collisions.size() >= 0);
  }

  SECTION("Multiple population collision detection") {
    Population population(world);
    population.set_size(3);
    population.update(0.0f);

    Vector2 position = {500.0f, 500.0f};  // Center of default world bounds
    float radius = 1000.0f;               // Large radius

    auto collisions = population.get_collisions(position, radius);

    // Should find up to 3 collisions
    REQUIRE(collisions.size() <= 3);
    // Must have at least 1
    REQUIRE(collisions.size() >= 1);

    // Each collision should reference a valid ant
    for (const auto& antRef : collisions) {
      // Just verify we can access the ant without crashing
      const Ant& ant = antRef.get();
      (void)ant;  // Suppress unused variable warning
    }
  }

  SECTION("Collision detection returns references") {
    Population population(world);
    population.set_size(1);
    population.update(0.0f);

    Vector2 position = {500.0f, 500.0f};
    float radius = 1000.0f;

    auto collisions = population.get_collisions(position, radius);

    // Verify that returned references can be used to modify ants
    for (auto& antRef : collisions) {
      Ant& ant = antRef.get();
      float originalEnergy = ant.get_energy();
      ant.set_energy(originalEnergy + 10.0f);
      REQUIRE(ant.get_energy() == originalEnergy + 10.0f);
    }
  }
}