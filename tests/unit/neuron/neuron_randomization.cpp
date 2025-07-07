#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "neuron.hpp"
#include "random_generator.hpp"

using Catch::Approx;

TEST_CASE("Neuron randomization", "[neuron]") {
  Neuron neuron;
  size_t weight_count = 3;
  neuron.set_input_count(weight_count);
  RandomGenerator generator;

  REQUIRE(neuron.get_bias() == 0.0f);

  SECTION("Randomize weights and bias") {
    neuron.randomize(generator);

    // Check that values are within reasonable ranges
    for (size_t i = 0; i < weight_count; ++i) {
      auto weight = neuron.get_input_weight(i);
      REQUIRE((weight > 0.0F || weight < 0.0F));
    }
    auto bias = neuron.get_bias();
    REQUIRE((bias < 0.0F || bias > 0.0F));  // bad test, but highly unlikely
  }
}