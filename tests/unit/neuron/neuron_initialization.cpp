#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "neuron.hpp"

using Catch::Approx;

TEST_CASE("Neuron initialization and basic operations", "[neuron]") {
  Neuron neuron;

  SECTION("Default initialization") {
    REQUIRE(neuron.get_input_count() == 0);
    REQUIRE(neuron.get_bias() == 0.0f);
  }

  SECTION("Setting input count") {
    neuron.set_input_count(3);
    REQUIRE(neuron.get_input_count() == 3);
  }

  SECTION("Setting and getting inputs") {
    neuron.set_input_count(2);
    neuron.set_input(0, 1.0f);
    neuron.set_input(1, 2.0f);

    REQUIRE(neuron.get_input(0) == 1.0f);
    REQUIRE(neuron.get_input(1) == 2.0f);
  }

  SECTION("Setting inputs using bulk setter and getter for outputs") {
    Neuron::ValueVector& inputs = neuron.get_inputs();
    neuron.set_input_count(2);
    inputs.at(0) = 0.125F;
    inputs.at(1) = -0.125f;

    const Neuron::ValueVector& const_inputs = neuron.get_inputs();

    REQUIRE(const_inputs.at(0) == 0.125F);
    REQUIRE(const_inputs.at(1) == -0.125F);
  }

  SECTION("Setting and getting weights") {
    neuron.set_input_count(2);
    neuron.set_input_weight(0, 0.5f);
    neuron.set_input_weight(1, -0.3f);

    REQUIRE(neuron.get_input_weight(0) == 0.5f);
    REQUIRE(neuron.get_input_weight(1) == -0.3f);
  }

  SECTION("Setting and getting bias") {
    neuron.set_bias(0.7f);
    REQUIRE(neuron.get_bias() == 0.7f);
  }
}
