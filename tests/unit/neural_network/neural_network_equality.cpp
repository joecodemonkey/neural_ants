#include <catch2/catch_test_macros.hpp>

#include "neural_network.hpp"

TEST_CASE("NeuralNetwork equality operator", "[neural_network]") {
  SECTION("Default networks are equal") {
    NeuralNetwork network1;
    NeuralNetwork network2;

    REQUIRE(network1 == network2);
    REQUIRE(network2 == network1);
  }

  SECTION("Identical configured networks are equal") {
    NeuralNetwork network1;

    std::vector<NeuralNetwork> networks(2);
    std::for_each(std::begin(networks), std::end(networks), [](auto& network) {
      network.set_input_count(10);
      network.set_hidden_layer_count(2);
      network.set_hidden_layer_neuron_count(15);
      network.set_output_neuron_count(5);
      network.randomize();
    });

    // Networks with different random weights should not be equal
    REQUIRE_FALSE(networks.at(0) == networks.at(1));
    REQUIRE_FALSE(networks.at(1) == networks.at(0));
  }

  SECTION("Copied networks are equal") {
    NeuralNetwork network1;
    network1.set_input_count(8);
    network1.set_hidden_layer_count(3);
    network1.set_hidden_layer_neuron_count(12);
    network1.set_output_neuron_count(4);
    network1.randomize();

    NeuralNetwork network2(network1);  // Copy constructor

    REQUIRE(network1 == network2);
    REQUIRE(network2 == network1);
  }

  SECTION("Assigned networks are equal") {
    NeuralNetwork network1;
    network1.set_input_count(6);
    network1.set_hidden_layer_count(1);
    network1.set_hidden_layer_neuron_count(8);
    network1.set_output_neuron_count(2);
    network1.randomize();

    NeuralNetwork network2;
    network2 = network1;  // Copy assignment

    REQUIRE(network1 == network2);
    REQUIRE(network2 == network1);
  }

  SECTION("Networks with different input counts are not equal") {
    NeuralNetwork network1;
    network1.set_input_count(5);

    NeuralNetwork network2;
    network2.set_input_count(10);

    REQUIRE_FALSE(network1 == network2);
    REQUIRE_FALSE(network2 == network1);
  }

  SECTION("Networks with different hidden layer counts are not equal") {
    NeuralNetwork network1;
    network1.set_hidden_layer_count(2);

    NeuralNetwork network2;
    network2.set_hidden_layer_count(3);

    REQUIRE_FALSE(network1 == network2);
    REQUIRE_FALSE(network2 == network1);
  }

  SECTION("Networks with different hidden layer neuron counts are not equal") {
    NeuralNetwork network1;
    network1.set_hidden_layer_neuron_count(10);

    NeuralNetwork network2;
    network2.set_hidden_layer_neuron_count(20);

    REQUIRE_FALSE(network1 == network2);
    REQUIRE_FALSE(network2 == network1);
  }

  SECTION("Networks with different output neuron counts are not equal") {
    NeuralNetwork network1;
    network1.set_output_neuron_count(2);

    NeuralNetwork network2;
    network2.set_output_neuron_count(5);

    REQUIRE_FALSE(network1 == network2);
    REQUIRE_FALSE(network2 == network1);
  }

  SECTION("Networks with different input values are not equal") {
    NeuralNetwork network1;
    network1.set_input_count(3);
    network1.set_input_values({1.0f, 2.0f, 3.0f});

    NeuralNetwork network2;
    network2.set_input_count(3);
    network2.set_input_values({1.0f, 2.0f, 4.0f});

    REQUIRE_FALSE(network1 == network2);
    REQUIRE_FALSE(network2 == network1);
  }

  SECTION("Networks with different ready states are not equal") {
    NeuralNetwork network1;
    network1.set_input_count(3);
    network1.set_input_values({1.0f, 2.0f, 3.0f});
    // network1 is not ready after setting new input values

    NeuralNetwork network2;
    network2.set_input_count(3);
    network2.set_input_values({1.0f, 2.0f, 3.0f});
    network2.get_output_values();  // This makes network2 ready

    REQUIRE_FALSE(network1 == network2);
    REQUIRE_FALSE(network2 == network1);
  }

  SECTION("Self equality") {
    NeuralNetwork network;
    network.set_input_count(5);
    network.set_hidden_layer_count(2);
    network.set_hidden_layer_neuron_count(10);
    network.set_output_neuron_count(3);
    network.randomize();

    REQUIRE(network == network);
  }

  SECTION("Networks with different neuron weights are not equal") {
    NeuralNetwork network1;
    network1.set_input_count(3);
    network1.set_hidden_layer_count(1);
    network1.set_hidden_layer_neuron_count(2);
    network1.set_output_neuron_count(1);
    network1.randomize();

    NeuralNetwork network2;
    network2.set_input_count(3);
    network2.set_hidden_layer_count(1);
    network2.set_hidden_layer_neuron_count(2);
    network2.set_output_neuron_count(1);
    network2.randomize();

    // Networks with different random weights should not be equal
    REQUIRE_FALSE(network1 == network2);
    REQUIRE_FALSE(network2 == network1);
  }
}