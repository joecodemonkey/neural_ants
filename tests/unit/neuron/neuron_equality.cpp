#include <algorithm>
#include <catch2/catch_test_macros.hpp>

#include "neuron.hpp"

TEST_CASE("Neuron equality operator", "[neuron]") {
  SECTION("identical neurons are equal") {
    std::vector<Neuron> neurons(2);
    std::for_each(std::begin(neurons), std::end(neurons), [](Neuron& neuron) {
      neuron.set_input_count(3);
      neuron.set_input(0, 1.0f);
      neuron.set_input(1, 2.0f);
      neuron.set_input(2, 3.0f);
      neuron.set_input_weight(0, 0.5f);
      neuron.set_input_weight(1, -0.3f);
      neuron.set_input_weight(2, 0.8f);
      neuron.set_bias(0.7f);
    });

    REQUIRE(neurons.at(0) == neurons.at(1));
    REQUIRE(neurons.at(1) == neurons.at(0));
  }

  SECTION("default constructed neurons are equal") {
    Neuron neuron1;
    Neuron neuron2;

    REQUIRE(neuron1 == neuron2);
    REQUIRE(neuron2 == neuron1);
  }

  SECTION("neurons with different bias are not equal") {
    Neuron neuron1;
    Neuron neuron2;

    neuron1.set_bias(0.5f);
    neuron2.set_bias(0.6f);

    REQUIRE_FALSE(neuron1 == neuron2);
    REQUIRE_FALSE(neuron2 == neuron1);
  }

  SECTION("neurons with different inputs are not equal") {
    Neuron neuron1;
    Neuron neuron2;

    neuron1.set_input_count(2);
    neuron1.set_input(0, 1.0f);
    neuron1.set_input(1, 2.0f);

    neuron2.set_input_count(2);
    neuron2.set_input(0, 1.0f);
    neuron2.set_input(1, 2.1f);  // Different input

    REQUIRE_FALSE(neuron1 == neuron2);
    REQUIRE_FALSE(neuron2 == neuron1);
  }

  SECTION("neurons with different weights are not equal") {
    Neuron neuron1;
    Neuron neuron2;

    neuron1.set_input_count(2);
    neuron1.set_input_weight(0, 0.5f);
    neuron1.set_input_weight(1, -0.3f);

    neuron2.set_input_count(2);
    neuron2.set_input_weight(0, 0.5f);
    neuron2.set_input_weight(1, -0.4f);  // Different weight

    REQUIRE_FALSE(neuron1 == neuron2);
    REQUIRE_FALSE(neuron2 == neuron1);
  }

  SECTION("neurons with different input counts are not equal") {
    Neuron neuron1;
    Neuron neuron2;

    neuron1.set_input_count(2);
    neuron2.set_input_count(3);

    REQUIRE_FALSE(neuron1 == neuron2);
    REQUIRE_FALSE(neuron2 == neuron1);
  }

  SECTION("neurons with different values are not equal") {
    std::vector<Neuron> neurons(2);
    std::for_each(std::begin(neurons), std::end(neurons), [](Neuron& neuron) {
      neuron.set_input_count(2);
      neuron.set_input(0, 1.0f);
      neuron.set_input(1, 2.0f);
      neuron.set_input_weight(0, 0.5f);
      neuron.set_input_weight(1, -0.3f);
      neuron.set_bias(0.7f);
      neuron.get_output();  // Calculate value
    });

    // Now change one neuron's value directly
    neurons[1].set_input(0, 1.1f);  // This will change the calculated value

    REQUIRE_FALSE(neurons.at(0) == neurons.at(1));
    REQUIRE_FALSE(neurons.at(1) == neurons.at(0));
  }

  SECTION("copy constructed neurons are equal") {
    Neuron neuron1;
    neuron1.randomize();

    Neuron neuron2(neuron1);  // Copy constructor

    REQUIRE(neuron1 == neuron2);
    REQUIRE(neuron2 == neuron1);
  }

  SECTION("assigned neurons are equal") {
    Neuron neuron1;
    neuron1.randomize();

    Neuron neuron2;
    neuron2 = neuron1;  // Assignment operator

    REQUIRE(neuron1 == neuron2);
    REQUIRE(neuron2 == neuron1);
  }

  SECTION("neurons with different threading settings are still equal") {
    std::vector<Neuron> neurons(2);
    std::for_each(std::begin(neurons), std::end(neurons), [](Neuron& neuron) {
      neuron.set_input_count(2);
      neuron.set_input(0, 1.0f);
      neuron.set_input(1, 2.0f);
      neuron.set_input_weight(0, 0.5f);
      neuron.set_input_weight(1, -0.3f);
      neuron.set_bias(0.7f);
    });

    // Enable threading on one neuron
    neurons[0].enable_threads();
    neurons[1].disable_threads();

    // Should still be equal since threading doesn't affect the stored values
    REQUIRE(neurons.at(0) == neurons.at(1));
    REQUIRE(neurons.at(1) == neurons.at(0));
  }

  SECTION("self equality") {
    Neuron neuron;
    neuron.set_input_count(2);
    neuron.set_input(0, 1.0f);
    neuron.set_input(1, 2.0f);
    neuron.set_input_weight(0, 0.5f);
    neuron.set_input_weight(1, -0.3f);
    neuron.set_bias(0.7f);

    REQUIRE(neuron == neuron);
  }

  SECTION("complex neuron configurations") {
    Neuron neuron1;
    Neuron neuron2;

    // Set up complex neurons with many inputs
    const size_t input_count = 10;
    neuron1.set_input_count(input_count);
    neuron2.set_input_count(input_count);

    for (size_t i = 0; i < input_count; ++i) {
      float input_val = static_cast<float>(i) * 0.1f;
      float weight_val = static_cast<float>(i) * 0.2f - 1.0f;

      neuron1.set_input(i, input_val);
      neuron1.set_input_weight(i, weight_val);

      neuron2.set_input(i, input_val);
      neuron2.set_input_weight(i, weight_val);
    }

    neuron1.set_bias(0.5f);
    neuron2.set_bias(0.5f);

    REQUIRE(neuron1 == neuron2);
    REQUIRE(neuron2 == neuron1);

    // Change one value and verify inequality
    neuron2.set_input(5, 0.6f);  // Different from 0.5f

    REQUIRE_FALSE(neuron1 == neuron2);
    REQUIRE_FALSE(neuron2 == neuron1);
  }
}