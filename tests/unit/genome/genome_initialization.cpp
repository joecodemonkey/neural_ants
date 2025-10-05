#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "genome.hpp"
#include "neural_network.hpp"

using Catch::Approx;

TEST_CASE("Genome initialization", "[genome]") {
  SECTION("Default constructor") {
    Genome genome;
    REQUIRE(genome.get_mutation_rate() == 0.1F);
    REQUIRE(genome.get_fitness() == 0.0F);

    const auto& network = genome.get_network();
    REQUIRE(network.get_input_count() == 100);                // Default input count
    REQUIRE(network.get_output_neuron_count() == 2);         // Default output count
    REQUIRE(network.get_hidden_layer_count() == 3);          // Default hidden layer count
    REQUIRE(network.get_hidden_layer_neuron_count() == 100);  // Default neurons per layer
  }

  SECTION("Copy constructor") {
    Genome original;
    original.set_mutation_rate(0.5F);
    original.set_fitness(100.0F);

    Genome copy(original);
    REQUIRE(copy.get_mutation_rate() == 0.5F);
    REQUIRE(copy.get_fitness() == 100.0F);

    // Verify neural network was copied
    const auto& original_network = original.get_network();
    const auto& copy_network = copy.get_network();
    REQUIRE(original_network.get_input_count() == copy_network.get_input_count());
    REQUIRE(original_network.get_output_neuron_count() == copy_network.get_output_neuron_count());
    REQUIRE(original_network.get_hidden_layer_count() == copy_network.get_hidden_layer_count());
    REQUIRE(original_network.get_hidden_layer_neuron_count() ==
            copy_network.get_hidden_layer_neuron_count());
  }

  SECTION("Move constructor") {
    Genome original;
    original.set_mutation_rate(0.5F);
    original.set_fitness(100.0F);

    Genome moved(std::move(original));
    REQUIRE(moved.get_mutation_rate() == 0.5F);
    REQUIRE(moved.get_fitness() == 100.0F);

    // Verify neural network was moved
    const auto& moved_network = moved.get_network();
    REQUIRE(moved_network.get_input_count() == 100);
    REQUIRE(moved_network.get_output_neuron_count() == 2);
    REQUIRE(moved_network.get_hidden_layer_count() == 3);
    REQUIRE(moved_network.get_hidden_layer_neuron_count() == 100);
  }

  SECTION("Randomize") {
    Genome genome;
    genome.randomize();

    // Verify neural network was randomized by checking its structure
    const auto& network = genome.get_network();
    REQUIRE(network.get_input_count() == 100);
    REQUIRE(network.get_output_neuron_count() == 2);
    REQUIRE(network.get_hidden_layer_count() == 3);
    REQUIRE(network.get_hidden_layer_neuron_count() == 100);
  }
}
