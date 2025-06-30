#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "neural_network.hpp"
#include "neuron.hpp"

using Catch::Approx;

TEST_CASE("Neural Network Serialization", "[neural_network]") {
  SECTION("Default Network Serialization") {
    NeuralNetwork network;

    network.set_input_count(5);
    network.set_hidden_layer_count(3);
    network.set_hidden_layer_neuron_count(50);
    network.set_output_neuron_count(2);
    // Set some input values and compute to populate output values
    NeuralNetwork::ValueVector inputs = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    network.set_input_values(inputs);
    auto output_values = network.get_output_values();

    auto json = network.to_json();

    // Test basic structure
    REQUIRE(json.contains("input_count"));
    REQUIRE(json.contains("hidden_layer_count"));
    REQUIRE(json.contains("hidden_layer_neuron_count"));
    REQUIRE(json.contains("output_neuron_count"));
    REQUIRE(json.contains("threaded"));
    REQUIRE(json.contains("validated"));
    REQUIRE(json.contains("ready"));
    REQUIRE(json.contains("input_values"));
    REQUIRE(json.contains("output_values"));
    REQUIRE(json.contains("hidden_layers"));
    REQUIRE(json.contains("output_layer"));

    // Test values
    REQUIRE(json["input_count"] == 5);
    REQUIRE(json["hidden_layer_count"] == 3);
    REQUIRE(json["hidden_layer_neuron_count"] == 50);
    REQUIRE(json["output_neuron_count"] == 2);
    REQUIRE(json["threaded"] == false);
    REQUIRE(json["validated"] == true);
    REQUIRE(json["ready"] == true);

    // Test input values
    REQUIRE(json["input_values"].size() == 5);
    REQUIRE(json["input_values"][0] == 1.0f);
    REQUIRE(json["input_values"][1] == 2.0f);
    REQUIRE(json["input_values"][2] == 3.0f);
    REQUIRE(json["input_values"][3] == 4.0f);
    REQUIRE(json["input_values"][4] == 5.0f);

    // Test output values
    REQUIRE(json["output_values"].size() == 2);

    // Test hidden layers structure
    REQUIRE(json["hidden_layers"].size() == 3);
    for (size_t i = 0; i < 3; ++i) {
      REQUIRE(json["hidden_layers"][i].size() == 50);
      for (size_t j = 0; j < 50; ++j) {
        REQUIRE(json["hidden_layers"][i][j].contains("weights"));
        REQUIRE(json["hidden_layers"][i][j].contains("bias"));
        REQUIRE(json["hidden_layers"][i][j].contains("threaded"));
      }
    }

    // Test output layer structure
    REQUIRE(json["output_layer"].size() == 2);
    for (size_t i = 0; i < 2; ++i) {
      REQUIRE(json["output_layer"][i].contains("weights"));
      REQUIRE(json["output_layer"][i].contains("bias"));
      REQUIRE(json["output_layer"][i].contains("threaded"));
    }
  }

  SECTION("Custom Network Serialization") {
    NeuralNetwork network;

    // Configure custom network
    network.set_input_count(10);
    network.set_hidden_layer_count(2);
    network.set_hidden_layer_neuron_count(20);
    network.set_output_neuron_count(5);
    network.enable_threads();

    // Set input values
    NeuralNetwork::ValueVector inputs = {
        0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f};
    network.set_input_values(inputs);

    // Randomize weights for testing
    network.randomize();

    // Compute to populate output values
    auto output_values = network.get_output_values();

    auto json = network.to_json();

    // Test custom values
    REQUIRE(json["input_count"] == 10);
    REQUIRE(json["hidden_layer_count"] == 2);
    REQUIRE(json["hidden_layer_neuron_count"] == 20);
    REQUIRE(json["output_neuron_count"] == 5);
    REQUIRE(json["threaded"] == true);

    // Test input values
    REQUIRE(json["input_values"].size() == 10);
    for (size_t i = 0; i < 10; ++i) {
      REQUIRE(json["input_values"][i] == Approx(0.1f * (i + 1)));
    }

    // Test output values
    REQUIRE(json["output_values"].size() == 5);

    // Test hidden layers
    REQUIRE(json["hidden_layers"].size() == 2);
    for (size_t i = 0; i < 2; ++i) {
      REQUIRE(json["hidden_layers"][i].size() == 20);
    }

    // Test output layer
    REQUIRE(json["output_layer"].size() == 5);
  }

  SECTION("Network Deserialization") {
    NeuralNetwork original;
    original.set_input_count(8);
    original.set_hidden_layer_count(1);
    original.set_hidden_layer_neuron_count(16);
    original.set_output_neuron_count(4);
    original.enable_threads();

    // Set input values and randomize
    NeuralNetwork::ValueVector inputs = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
    original.set_input_values(inputs);
    original.randomize();
    original.get_output_values();

    // Serialize and deserialize
    auto json = original.to_json();
    NeuralNetwork reconstructed(json);
    REQUIRE(reconstructed == original);
  }

  SECTION("Empty Network Serialization") {
    NeuralNetwork network;
    network.set_input_count(0);
    network.set_hidden_layer_count(0);
    network.set_output_neuron_count(0);

    auto json = network.to_json();

    REQUIRE(json["input_count"] == 0);
    REQUIRE(json["hidden_layer_count"] == 0);
    REQUIRE(json["hidden_layer_neuron_count"] == 50);  // Default value
    REQUIRE(json["output_neuron_count"] == 0);
    REQUIRE(json["input_values"].size() == 0);
    REQUIRE(json["output_values"].size() == 0);
    REQUIRE(json["hidden_layers"].size() == 0);
    REQUIRE(json["output_layer"].size() == 0);
  }
}