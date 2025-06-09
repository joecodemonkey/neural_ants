#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "neural_network.hpp"
#include "neuron.hpp"

using Catch::Approx;

TEST_CASE("Neural Network Randomization", "[neural_network]") {
  NeuralNetwork network;
  network.set_input_count(3);
  network.set_hidden_layer_count(2);
  network.set_hidden_layer_neuron_count(4);
  network.set_output_neuron_count(2);

  SECTION("Hidden Layers Randomization") {
    network.randomize();

    // Check each hidden layer
    for (size_t layer_idx = 0; layer_idx < network.get_hidden_layer_count(); ++layer_idx) {
      const auto& layer = network.get_hidden_layer(layer_idx);

      // Check each neuron in the layer
      for (const auto& neuron : layer) {
        // Check weights
        for (size_t weight_idx = 0; weight_idx < neuron.get_input_count(); ++weight_idx) {
          auto weight = neuron.get_input_weight(weight_idx);
          REQUIRE((weight >= -1.0F && weight <= 1.0F));
        }

        // Check bias
        auto bias = neuron.get_bias();
        REQUIRE((bias >= -1.0F && bias <= 1.0F));
      }
    }
  }

  SECTION("Output Layer Randomization") {
    network.randomize();

    const auto& output_layer = network.get_output_layer();

    // Check each neuron in the output layer
    for (const auto& neuron : output_layer) {
      // Check weights
      for (size_t weight_idx = 0; weight_idx < neuron.get_input_count(); ++weight_idx) {
        auto weight = neuron.get_input_weight(weight_idx);
        REQUIRE((weight >= -1.0F && weight <= 1.0F));
      }

      // Check bias
      auto bias = neuron.get_bias();
      REQUIRE((bias >= -1.0F && bias <= 1.0F));
    }
  }

  SECTION("Multiple Randomizations") {
    // Store initial random values
    network.randomize();
    std::vector<std::vector<float>> initial_weights;
    std::vector<float> initial_biases;

    // Store weights and biases from all layers
    for (size_t layer_idx = 0; layer_idx < network.get_hidden_layer_count(); ++layer_idx) {
      const auto& layer = network.get_hidden_layer(layer_idx);
      for (const auto& neuron : layer) {
        std::vector<float> neuron_weights;
        for (size_t i = 0; i < neuron.get_input_count(); ++i) {
          neuron_weights.push_back(neuron.get_input_weight(i));
        }
        initial_weights.push_back(neuron_weights);
        initial_biases.push_back(neuron.get_bias());
      }
    }

    // Store output layer weights and biases
    for (const auto& neuron : network.get_output_layer()) {
      std::vector<float> neuron_weights;
      for (size_t i = 0; i < neuron.get_input_count(); ++i) {
        neuron_weights.push_back(neuron.get_input_weight(i));
      }
      initial_weights.push_back(neuron_weights);
      initial_biases.push_back(neuron.get_bias());
    }

    // Randomize again
    network.randomize();

    // Compare with new values
    size_t weight_idx = 0;
    size_t bias_idx = 0;

    // Check hidden layers
    for (size_t layer_idx = 0; layer_idx < network.get_hidden_layer_count(); ++layer_idx) {
      const auto& layer = network.get_hidden_layer(layer_idx);
      for (const auto& neuron : layer) {
        // Check weights
        for (size_t i = 0; i < neuron.get_input_count(); ++i) {
          REQUIRE(neuron.get_input_weight(i) != initial_weights[weight_idx][i]);
        }
        weight_idx++;

        // Check bias
        REQUIRE(neuron.get_bias() != initial_biases[bias_idx]);
        bias_idx++;
      }
    }

    // Check output layer
    for (const auto& neuron : network.get_output_layer()) {
      // Check weights
      for (size_t i = 0; i < neuron.get_input_count(); ++i) {
        REQUIRE(neuron.get_input_weight(i) != initial_weights[weight_idx][i]);
      }
      weight_idx++;

      // Check bias
      REQUIRE(neuron.get_bias() != initial_biases[bias_idx]);
      bias_idx++;
    }
  }
}
