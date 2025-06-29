#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "genome.hpp"
#include "neural_network.hpp"

using Catch::Approx;

TEST_CASE("Genome serialization and deserialization", "[genome][serialization]") {
  SECTION("Default genome serialization") {
    Genome genome;
    auto json = genome.to_json();

    REQUIRE(json.contains("network"));
    REQUIRE(json.contains("mutation_rate"));
    REQUIRE(json.contains("fitness"));

    REQUIRE(json["mutation_rate"] == 0.1f);
    REQUIRE(json["fitness"] == 0.0f);
    REQUIRE(json["network"].is_object());
  }

  SECTION("Round-trip serialization and deserialization") {
    Genome original;

    // Configure the neural network
    auto& network = const_cast<NeuralNetwork&>(original.get_network());
    network.set_input_count(4);
    network.set_hidden_layer_count(2);
    network.set_hidden_layer_neuron_count(6);
    network.set_output_neuron_count(3);
    network.randomize();

    // Set genome properties
    original.set_mutation_rate(0.3);
    original.set_fitness(0.75);

    auto json = original.to_json();
    Genome deserialized(json);

    // Use equality operator for comprehensive comparison
    REQUIRE(original == deserialized);
    REQUIRE(deserialized == original);
  }

  SECTION("Round-trip with default genome") {
    Genome original;
    auto json = original.to_json();
    Genome deserialized(json);

    REQUIRE(original == deserialized);
  }

  SECTION("Round-trip with randomized genome") {
    Genome original;
    original.randomize();
    original.set_mutation_rate(0.5);
    original.set_fitness(100.0);

    auto json = original.to_json();
    Genome deserialized(json);

    REQUIRE(original == deserialized);
  }
}