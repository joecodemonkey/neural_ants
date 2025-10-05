#include <catch2/catch_test_macros.hpp>

#include "neural_network.hpp"
#include "neuron.hpp"

TEST_CASE("Neural Network Initialization", "[neural_network]") {
  NeuralNetwork network;

  SECTION("Default Constructor") {
    // Test default values
    REQUIRE(network.get_input_count() == 100);
    REQUIRE(network.get_hidden_layer_count() == 2);
    REQUIRE(network.get_hidden_layer_neuron_count() == 16);
    REQUIRE(network.get_output_neuron_count() == 2);
  }

  SECTION("Input Values Initialization") {
    // Test input values initialization
    const auto& input_values = network.get_input_values();
    REQUIRE(input_values.size() == 100);

    // All input values should be initialized to 0
    for (const auto& value : input_values) {
      REQUIRE(value == 0.0);
    }
  }

  SECTION("Hidden Layers Initialization") {
    // Test hidden layers initialization
    REQUIRE(network.get_hidden_layer_count() == 2);

    // Check each hidden layer
    for (size_t i = 0; i < 2; ++i) {  // Now 2 hidden layers
      const auto& layer = network.get_hidden_layer(i);
      REQUIRE(layer.size() == 16);
    }
  }

  SECTION("Output Layer Initialization") {
    // Test output layer initialization
    const auto& output_layer = network.get_output_layer();
    REQUIRE(output_layer.size() == 2);

    // Test output values
    auto output_values = network.get_output_values();
    REQUIRE(output_values.size() == 2);

    // All output values should be initialized to 0
    for (const auto& value : output_values) {
      REQUIRE(value == 0.0);
    }
  }

  SECTION("Custom Initialization") {
    // Test custom initialization
    network.set_input_count(50);
    network.set_hidden_layer_count(2);
    network.set_hidden_layer_neuron_count(75);
    network.set_output_neuron_count(3);

    REQUIRE(network.get_input_count() == 50);
    REQUIRE(network.get_hidden_layer_count() == 2);
    REQUIRE(network.get_hidden_layer_neuron_count() == 75);
    REQUIRE(network.get_output_neuron_count() == 3);

    // Verify hidden layers were updated
    for (size_t i = 0; i < 2; ++i) {
      const auto& layer = network.get_hidden_layer(i);
      REQUIRE(layer.size() == 75);
      for (auto neuron : layer) {
        if (i == 0) {
          REQUIRE(neuron.get_input_count() == 50);
        } else {
          REQUIRE(neuron.get_input_count() == 75);
        }
      }
    }

    // Verify output layer was updated
    const auto& output_layer = network.get_output_layer();
    REQUIRE(output_layer.size() == 3);
    for (auto neuron : output_layer) {
      REQUIRE(neuron.get_input_count() == 75);
      REQUIRE(neuron.get_inputs().size() == 75);
    }
  }
}
