#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "neuron.hpp"

using Catch::Approx;

TEST_CASE("Neuron output calculation", "[neuron]") {
  Neuron neuron;

  SECTION("Single input calculation") {
    neuron.set_input_count(1);
    neuron.set_input(0, 1.0f);
    neuron.set_input_weight(0, 0.5f);
    neuron.set_bias(0.2f);

    // Expected: tanh(1.0 * 0.5 + 0.2) = tanh(0.7)
    REQUIRE(neuron.get_output() == Approx(std::tanh(0.7f)));
  }

  SECTION("Multiple input calculation") {
    neuron.set_input_count(2);
    neuron.set_input(0, 1.0f);
    neuron.set_input(1, -0.5f);
    neuron.set_input_weight(0, 0.5f);
    neuron.set_input_weight(1, 0.3f);
    neuron.set_bias(0.2f);

    auto output_value = neuron.get_bias() + neuron.get_input_weight(0) * neuron.get_input(0);
    output_value += neuron.get_input_weight(1) * neuron.get_input(1);
    REQUIRE(neuron.get_output() == Approx(std::tanh(output_value)));
    REQUIRE(neuron.get_output() == Approx(std::tanh(0.55f)));
    // Expected: tanh(1.0 * 0.5 + (-0.5) * 0.3 + 0.2) = tanh(0.55)
  }
}