#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "neural_network.hpp"
#include "neuron.hpp"

using Catch::Approx;

TEST_CASE("Neural Network Calculations", "[neural_network]") {
  NeuralNetwork network;

  SECTION("Input Value Setting") {
    NeuralNetwork::ValueVector test_inputs(100, 0.5);
    network.set_input_values(test_inputs);

    const auto& input_values = network.get_input_values();
    REQUIRE(input_values.size() == 100);
    for (const auto& value : input_values) {
      REQUIRE(value == 0.5);
    }
  }

  SECTION("Forward Propagation") {
    // Set up a simple network with known weights
    network.set_input_count(2);
    network.set_hidden_layer_count(1);
    network.set_hidden_layer_neuron_count(2);
    network.set_output_neuron_count(1);

    // Set input values
    NeuralNetwork::ValueVector inputs = {0.5, 0.3};
    network.set_input_values(inputs);

    // Test forward propagation by getting output values
    const auto& output_values = network.get_output_values();
    REQUIRE(output_values.size() == 1);
    REQUIRE(output_values[0] >= 0.0);
    REQUIRE(output_values[0] <= 1.0);  // Assuming sigmoid activation
  }

  SECTION("Multiple Hidden Layers Calculation") {
    // Set up a network with multiple hidden layers
    network.set_input_count(3);
    network.set_hidden_layer_count(2);
    network.set_hidden_layer_neuron_count(4);
    network.set_output_neuron_count(2);

    // Set input values
    NeuralNetwork::ValueVector inputs = {0.2, 0.4, 0.6};
    network.set_input_values(inputs);

    // Test forward propagation by getting output values
    const auto& output_values = network.get_output_values();
    REQUIRE(output_values.size() == 2);
    for (const auto& value : output_values) {
      REQUIRE(value >= 0.0);
      REQUIRE(value <= 1.0);  // Assuming sigmoid activation
    }
  }

  SECTION("Consistent Calculations") {
    // Test that the same inputs produce the same outputs
    NeuralNetwork::ValueVector inputs(50, 0.5);
    network.set_input_values(inputs);

    auto first_output = network.get_output_values();
    auto second_output = network.get_output_values();

    REQUIRE(first_output.size() == second_output.size());
    for (size_t i = 0; i < first_output.size(); ++i) {
      REQUIRE(first_output[i] == second_output[i]);
    }
  }

  SECTION("Input Range Handling") {
    // Test network behavior with different input ranges
    NeuralNetwork::ValueVector inputs(50);
    for (size_t i = 0; i < inputs.size(); ++i) {
      inputs[i] = static_cast<Neuron::Value>(i) / 100.0;  // Values from 0 to 0.99
    }
    network.set_input_values(inputs);

    const auto& output_values = network.get_output_values();

    REQUIRE(output_values.size() == 2);
    for (const auto& value : output_values) {
      REQUIRE(value >= 0.0);
      REQUIRE(value <= 1.0);
    }
  }

  SECTION("Validated Calculation") {
    // Set up network with explicit architecture
    network.set_input_count(2);
    network.set_hidden_layer_count(2);
    network.set_hidden_layer_neuron_count(2);
    network.set_output_neuron_count(2);

    // Create layers with explicit weights and biases
    NeuralNetwork::Layer first_hidden(2);
    // First neuron in first hidden layer
    first_hidden[0].set_input_count(2);
    first_hidden[0].set_input_weight(0, 0.5);   // weight for first input
    first_hidden[0].set_input_weight(1, -0.3);  // weight for second input
    first_hidden[0].set_bias(0.2);
    // Second neuron in first hidden layer
    first_hidden[1].set_input_count(2);
    first_hidden[1].set_input_weight(0, -0.4);  // weight for first input
    first_hidden[1].set_input_weight(1, 0.6);   // weight for second input
    first_hidden[1].set_bias(-0.1);
    network.set_hidden_layer(0, first_hidden);

    // Second hidden layer
    NeuralNetwork::Layer second_hidden(2);
    // First neuron in second hidden layer
    second_hidden[0].set_input_count(2);
    second_hidden[0].set_input_weight(0, 0.3);  // weight for first input
    second_hidden[0].set_input_weight(1, 0.7);  // weight for second input
    second_hidden[0].set_bias(0.1);
    // Second neuron in second hidden layer
    second_hidden[1].set_input_count(2);
    second_hidden[1].set_input_weight(0, -0.2);  // weight for first input
    second_hidden[1].set_input_weight(1, 0.4);   // weight for second input
    second_hidden[1].set_bias(0.3);
    network.set_hidden_layer(1, second_hidden);

    // Output layer
    NeuralNetwork::Layer output_layer(2);
    // First output neuron
    output_layer[0].set_input_count(2);
    output_layer[0].set_input_weight(0, 0.6);   // weight for first input
    output_layer[0].set_input_weight(1, -0.5);  // weight for second input
    output_layer[0].set_bias(0.2);
    // Second output neuron
    output_layer[1].set_input_count(2);
    output_layer[1].set_input_weight(0, -0.3);  // weight for first input
    output_layer[1].set_input_weight(1, 0.8);   // weight for second input
    output_layer[1].set_bias(-0.1);
    network.set_output_layer(output_layer);

    // Set input values
    NeuralNetwork::ValueVector inputs = {0.5, 0.3};
    network.set_input_values(inputs);

    // Calculate expected outputs manually
    // First hidden layer calculations
    float h1_1 = tanh(0.5 * 0.5 + 0.3 * (-0.3) + 0.2);     // First neuron
    float h1_2 = tanh(0.5 * (-0.4) + 0.3 * 0.6 + (-0.1));  // Second neuron

    // Second hidden layer calculations
    float h2_1 = tanh(h1_1 * 0.3 + h1_2 * 0.7 + 0.1);     // First neuron
    float h2_2 = tanh(h1_1 * (-0.2) + h1_2 * 0.4 + 0.3);  // Second neuron

    // Output layer calculations
    float out1 = tanh(h2_1 * 0.6 + h2_2 * (-0.5) + 0.2);     // First output
    float out2 = tanh(h2_1 * (-0.3) + h2_2 * 0.8 + (-0.1));  // Second output

    // Get actual network outputs
    const auto& output_values = network.get_output_values();
    REQUIRE(output_values.size() == 2);

    // Compare expected and actual outputs
    REQUIRE(output_values[0] == Approx(out1).margin(0.0001));
    REQUIRE(output_values[1] == Approx(out2).margin(0.0001));
  }
}
