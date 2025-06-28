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
    auto original_outputs = original.get_output_values();

    // Serialize and deserialize
    auto json = original.to_json();
    NeuralNetwork reconstructed(json);

    // Test basic properties
    REQUIRE(reconstructed.get_input_count() == original.get_input_count());
    REQUIRE(reconstructed.get_hidden_layer_count() == original.get_hidden_layer_count());
    REQUIRE(reconstructed.get_hidden_layer_neuron_count() ==
            original.get_hidden_layer_neuron_count());
    REQUIRE(reconstructed.get_output_neuron_count() == original.get_output_neuron_count());

    // Test input values
    const auto& original_inputs = original.get_input_values();
    const auto& reconstructed_inputs = reconstructed.get_input_values();
    REQUIRE(reconstructed_inputs.size() == original_inputs.size());
    for (size_t i = 0; i < original_inputs.size(); ++i) {
      REQUIRE(reconstructed_inputs[i] == original_inputs[i]);
    }

    // Test output values
    auto reconstructed_outputs = reconstructed.get_output_values();
    REQUIRE(reconstructed_outputs.size() == original_outputs.size());
    for (size_t i = 0; i < original_outputs.size(); ++i) {
      REQUIRE(reconstructed_outputs[i] == Approx(original_outputs[i]));
    }

    // Test hidden layers
    for (size_t i = 0; i < original.get_hidden_layer_count(); ++i) {
      const auto& original_layer = original.get_hidden_layer(i);
      const auto& reconstructed_layer = reconstructed.get_hidden_layer(i);
      REQUIRE(reconstructed_layer.size() == original_layer.size());

      for (size_t j = 0; j < original_layer.size(); ++j) {
        const auto& original_neuron = original_layer[j];
        const auto& reconstructed_neuron = reconstructed_layer[j];

        REQUIRE(reconstructed_neuron.get_input_count() == original_neuron.get_input_count());
        REQUIRE(reconstructed_neuron.get_bias() == Approx(original_neuron.get_bias()));

        // Test weights individually
        for (size_t k = 0; k < original_neuron.get_input_count(); ++k) {
          REQUIRE(reconstructed_neuron.get_input_weight(k) ==
                  Approx(original_neuron.get_input_weight(k)));
        }
      }
    }

    // Test output layer
    const auto& original_output_layer = original.get_output_layer();
    const auto& reconstructed_output_layer = reconstructed.get_output_layer();
    REQUIRE(reconstructed_output_layer.size() == original_output_layer.size());

    for (size_t i = 0; i < original_output_layer.size(); ++i) {
      const auto& original_neuron = original_output_layer[i];
      const auto& reconstructed_neuron = reconstructed_output_layer[i];

      REQUIRE(reconstructed_neuron.get_input_count() == original_neuron.get_input_count());
      REQUIRE(reconstructed_neuron.get_bias() == Approx(original_neuron.get_bias()));

      // Test weights individually
      for (size_t j = 0; j < original_neuron.get_input_count(); ++j) {
        REQUIRE(reconstructed_neuron.get_input_weight(j) ==
                Approx(original_neuron.get_input_weight(j)));
      }
    }
  }

  SECTION("Network Roundtrip with Computation") {
    NeuralNetwork network;
    network.set_input_count(6);
    network.set_hidden_layer_count(2);
    network.set_hidden_layer_neuron_count(12);
    network.set_output_neuron_count(3);

    // Set input values
    NeuralNetwork::ValueVector inputs = {0.5f, 0.3f, 0.8f, 0.2f, 0.9f, 0.1f};
    network.set_input_values(inputs);

    // Randomize and compute
    network.randomize();
    auto original_outputs = network.get_output_values();

    // Serialize and deserialize
    auto json = network.to_json();
    NeuralNetwork reconstructed(json);

    // Set same input values and compute
    reconstructed.set_input_values(inputs);
    auto reconstructed_outputs = reconstructed.get_output_values();

    // Results should be identical
    REQUIRE(reconstructed_outputs.size() == original_outputs.size());
    for (size_t i = 0; i < original_outputs.size(); ++i) {
      REQUIRE(reconstructed_outputs[i] == Approx(original_outputs[i]));
    }
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

  SECTION("Threaded Network Serialization") {
    NeuralNetwork network;
    network.set_input_count(4);
    network.set_hidden_layer_count(1);
    network.set_hidden_layer_neuron_count(8);
    network.set_output_neuron_count(2);
    network.enable_threads();

    NeuralNetwork::ValueVector inputs = {1.0f, 2.0f, 3.0f, 4.0f};
    network.set_input_values(inputs);
    network.randomize();
    network.get_output_values();  // Compute

    auto json = network.to_json();

    REQUIRE(json["threaded"] == true);

    // Deserialize and verify threading is preserved
    NeuralNetwork reconstructed(json);
    REQUIRE(reconstructed.get_input_count() == 4);
    REQUIRE(reconstructed.get_hidden_layer_count() == 1);
    REQUIRE(reconstructed.get_hidden_layer_neuron_count() == 8);
    REQUIRE(reconstructed.get_output_neuron_count() == 2);

    // Test that computation still works
    reconstructed.set_input_values(inputs);
    auto outputs = reconstructed.get_output_values();
    REQUIRE(outputs.size() == 2);
  }
}