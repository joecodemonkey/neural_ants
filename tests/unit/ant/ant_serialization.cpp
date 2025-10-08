#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "ant.hpp"
#include "genome.hpp"
#include "world.hpp"

using Catch::Approx;

TEST_CASE("Ant serialization and deserialization", "[ant][serialization]") {
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

  SECTION("Basic ant creation and serialization") {
    Genome genome = create_minimal_genome();
    Ant ant(world, genome);

    // Just test that we can create an ant and serialize it
    auto json = ant.to_json();

    REQUIRE(json.contains("position"));
    REQUIRE(json.contains("velocity"));
    REQUIRE(json.contains("bounds"));
    REQUIRE(json.contains("radius"));
    REQUIRE(json.contains("dead"));
    REQUIRE(json.contains("frozen"));
    REQUIRE(json.contains("texture_width"));
    REQUIRE(json.contains("texture_height"));
    REQUIRE(json.contains("energy"));
    REQUIRE(json.contains("life_span"));
    REQUIRE(json.contains("genome"));
  }

  SECTION("Default ant serialization") {
    Genome genome = create_minimal_genome();
    Ant ant(world, genome);
    auto json = ant.to_json();

    // Test default values
    REQUIRE(json["dead"] == false);
    REQUIRE(json["frozen"] == false);
    REQUIRE(json["texture_width"] == 16.0f);
    REQUIRE(json["texture_height"] == 16.0f);
    REQUIRE(json["energy"] == 1000.0f);
    REQUIRE(json["life_span"] == 0.0f);
    REQUIRE(json["radius"] == Approx(11.313708305358887).margin(0.01));  // Now computed from texture dimensions
    REQUIRE(json["genome"].is_object());
  }

  SECTION("Serialization with custom properties") {
    Genome genome = create_minimal_genome();
    Ant ant(world, genome);

    ant.set_position({15.5f, 25.3f});
    ant.set_energy(750.0f);
    ant.set_dead(false);

    auto json = ant.to_json();

    // Test position
    REQUIRE(json["position"]["x"] == 15.5f);
    REQUIRE(json["position"]["y"] == 25.3f);

    // Test other properties
    REQUIRE(json["energy"] == 750.0f);
    REQUIRE(json["texture_width"] == 16.0f);
    REQUIRE(json["texture_height"] == 16.0f);
    REQUIRE(json["dead"] == false);
    REQUIRE(json["life_span"] == 0.0f);

    // Test bounds structure
    REQUIRE(json["bounds"].contains("x"));
    REQUIRE(json["bounds"].contains("y"));
    REQUIRE(json["bounds"].contains("width"));
    REQUIRE(json["bounds"].contains("height"));

    // Test genome is serialized
    REQUIRE(json["genome"].is_object());
  }
}