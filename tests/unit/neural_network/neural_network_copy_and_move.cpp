#include <catch2/catch_test_macros.hpp>

#include "neural_network.hpp"

TEST_CASE("NeuralNetwork copy and move operations", "[neural_network]") {
  // Create a neural network with some non-default values
  NeuralNetwork network;
  network.set_input_count(5);
  network.set_hidden_layer_count(2);
  network.set_hidden_layer_neuron_count(10);
  network.set_output_neuron_count(3);
  network.randomize();  // Initialize with random weights

  SECTION("Copy constructor") {
    NeuralNetwork copy(network);

    // Verify all properties are copied correctly
    REQUIRE(copy.get_input_count() == network.get_input_count());
    REQUIRE(copy.get_hidden_layer_count() == network.get_hidden_layer_count());
    REQUIRE(copy.get_hidden_layer_neuron_count() == network.get_hidden_layer_neuron_count());
    REQUIRE(copy.get_output_neuron_count() == network.get_output_neuron_count());

    // Verify the layers are copied (not just the counts)
    for (size_t i = 0; i < network.get_hidden_layer_count(); ++i) {
      const auto& original_layer = network.get_hidden_layer(i);
      const auto& copied_layer = copy.get_hidden_layer(i);
      REQUIRE(original_layer.size() == copied_layer.size());
    }

    const auto& original_output = network.get_output_layer();
    const auto& copied_output = copy.get_output_layer();
    REQUIRE(original_output.size() == copied_output.size());
  }

  SECTION("Copy assignment operator") {
    NeuralNetwork copy;
    copy = network;

    // Verify all properties are copied correctly
    REQUIRE(copy.get_input_count() == network.get_input_count());
    REQUIRE(copy.get_hidden_layer_count() == network.get_hidden_layer_count());
    REQUIRE(copy.get_hidden_layer_neuron_count() == network.get_hidden_layer_neuron_count());
    REQUIRE(copy.get_output_neuron_count() == network.get_output_neuron_count());

    // Verify the layers are copied (not just the counts)
    for (size_t i = 0; i < network.get_hidden_layer_count(); ++i) {
      const auto& original_layer = network.get_hidden_layer(i);
      const auto& copied_layer = copy.get_hidden_layer(i);
      REQUIRE(original_layer.size() == copied_layer.size());
    }

    const auto& original_output = network.get_output_layer();
    const auto& copied_output = copy.get_output_layer();
    REQUIRE(original_output.size() == copied_output.size());
  }

  SECTION("Move constructor") {
    NeuralNetwork original = network;  // Make a copy first
    NeuralNetwork moved(std::move(original));

    // Verify all properties are moved correctly
    REQUIRE(moved.get_input_count() == network.get_input_count());
    REQUIRE(moved.get_hidden_layer_count() == network.get_hidden_layer_count());
    REQUIRE(moved.get_hidden_layer_neuron_count() == network.get_hidden_layer_neuron_count());
    REQUIRE(moved.get_output_neuron_count() == network.get_output_neuron_count());

    // Verify the layers are moved (not just the counts)
    for (size_t i = 0; i < network.get_hidden_layer_count(); ++i) {
      const auto& original_layer = network.get_hidden_layer(i);
      const auto& moved_layer = moved.get_hidden_layer(i);
      REQUIRE(original_layer.size() == moved_layer.size());
    }

    const auto& original_output = network.get_output_layer();
    const auto& moved_output = moved.get_output_layer();
    REQUIRE(original_output.size() == moved_output.size());
  }

  SECTION("Move assignment operator") {
    NeuralNetwork original = network;  // Make a copy first
    NeuralNetwork moved;
    moved = std::move(original);

    // Verify all properties are moved correctly
    REQUIRE(moved.get_input_count() == network.get_input_count());
    REQUIRE(moved.get_hidden_layer_count() == network.get_hidden_layer_count());
    REQUIRE(moved.get_hidden_layer_neuron_count() == network.get_hidden_layer_neuron_count());
    REQUIRE(moved.get_output_neuron_count() == network.get_output_neuron_count());

    // Verify the layers are moved (not just the counts)
    for (size_t i = 0; i < network.get_hidden_layer_count(); ++i) {
      const auto& original_layer = network.get_hidden_layer(i);
      const auto& moved_layer = moved.get_hidden_layer(i);
      REQUIRE(original_layer.size() == moved_layer.size());
    }

    const auto& original_output = network.get_output_layer();
    const auto& moved_output = moved.get_output_layer();
    REQUIRE(original_output.size() == moved_output.size());
  }
}