#include <catch2/catch_test_macros.hpp>

#include "neuron.hpp"

TEST_CASE("Neuron equality operator", "[neuron]") {
  SECTION("identical neurons are equal") {
    Neuron neuron1;
    Neuron neuron2;

    // Set up identical neurons
    neuron1.set_input_count(3);
    neuron1.set_input(0, 1.0f);
    neuron1.set_input(1, 2.0f);
    neuron1.set_input(2, 3.0f);
    neuron1.set_input_weight(0, 0.5f);
    neuron1.set_input_weight(1, -0.3f);
    neuron1.set_input_weight(2, 0.8f);
    neuron1.set_bias(0.7f);

    neuron2.set_input_count(3);
    neuron2.set_input(0, 1.0f);
    neuron2.set_input(1, 2.0f);
    neuron2.set_input(2, 3.0f);
    neuron2.set_input_weight(0, 0.5f);
    neuron2.set_input_weight(1, -0.3f);
    neuron2.set_input_weight(2, 0.8f);
    neuron2.set_bias(0.7f);

    REQUIRE(neuron1 == neuron2);
    REQUIRE(neuron2 == neuron1);
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
    Neuron neuron1;
    Neuron neuron2;

    // Set up neurons with same inputs/weights/bias but different calculated values
    neuron1.set_input_count(2);
    neuron1.set_input(0, 1.0f);
    neuron1.set_input(1, 2.0f);
    neuron1.set_input_weight(0, 0.5f);
    neuron1.set_input_weight(1, -0.3f);
    neuron1.set_bias(0.7f);
    neuron1.get_output();  // Calculate value

    neuron2.set_input_count(2);
    neuron2.set_input(0, 1.0f);
    neuron2.set_input(1, 2.0f);
    neuron2.set_input_weight(0, 0.5f);
    neuron2.set_input_weight(1, -0.3f);
    neuron2.set_bias(0.7f);
    neuron2.get_output();  // Calculate value

    // Now change one neuron's value directly
    neuron2.set_input(0, 1.1f);  // This will change the calculated value

    REQUIRE_FALSE(neuron1 == neuron2);
    REQUIRE_FALSE(neuron2 == neuron1);
  }

  SECTION("neurons with near-equal floating point values are equal") {
    Neuron neuron1;
    Neuron neuron2;

    neuron1.set_input_count(1);
    neuron1.set_input(0, 1.0f);
    neuron1.set_input_weight(0, 0.5f);
    neuron1.set_bias(0.7f);

    neuron2.set_input_count(1);
    neuron2.set_input(0, 1.0f + std::numeric_limits<float>::epsilon());
    neuron2.set_input_weight(0, 0.5f);
    neuron2.set_bias(0.7f);

    // Should be equal due to floating point tolerance in Util::equal
    REQUIRE(neuron1 == neuron2);
    REQUIRE(neuron2 == neuron1);
  }

  SECTION("neurons with significantly different floating point values are not equal") {
    Neuron neuron1;
    Neuron neuron2;

    neuron1.set_input_count(1);
    neuron1.set_input(0, 1.0f);
    neuron1.set_input_weight(0, 0.5f);
    neuron1.set_bias(0.7f);

    neuron2.set_input_count(1);
    neuron2.set_input(0, 1.0f + 2.0f * std::numeric_limits<float>::epsilon());
    neuron2.set_input_weight(0, 0.5f);
    neuron2.set_bias(0.7f);

    // Should not be equal due to floating point tolerance
    REQUIRE_FALSE(neuron1 == neuron2);
    REQUIRE_FALSE(neuron2 == neuron1);
  }

  SECTION("copy constructed neurons are equal") {
    Neuron neuron1;
    neuron1.set_input_count(3);
    neuron1.set_input(0, 1.0f);
    neuron1.set_input(1, 2.0f);
    neuron1.set_input(2, 3.0f);
    neuron1.set_input_weight(0, 0.5f);
    neuron1.set_input_weight(1, -0.3f);
    neuron1.set_input_weight(2, 0.8f);
    neuron1.set_bias(0.7f);

    Neuron neuron2(neuron1);  // Copy constructor

    REQUIRE(neuron1 == neuron2);
    REQUIRE(neuron2 == neuron1);
  }

  SECTION("assigned neurons are equal") {
    Neuron neuron1;
    neuron1.set_input_count(2);
    neuron1.set_input(0, 1.0f);
    neuron1.set_input(1, 2.0f);
    neuron1.set_input_weight(0, 0.5f);
    neuron1.set_input_weight(1, -0.3f);
    neuron1.set_bias(0.7f);

    Neuron neuron2;
    neuron2 = neuron1;  // Assignment operator

    REQUIRE(neuron1 == neuron2);
    REQUIRE(neuron2 == neuron1);
  }

  SECTION("neurons with different threading settings are still equal") {
    Neuron neuron1;
    Neuron neuron2;

    neuron1.set_input_count(2);
    neuron1.set_input(0, 1.0f);
    neuron1.set_input(1, 2.0f);
    neuron1.set_input_weight(0, 0.5f);
    neuron1.set_input_weight(1, -0.3f);
    neuron1.set_bias(0.7f);

    neuron2.set_input_count(2);
    neuron2.set_input(0, 1.0f);
    neuron2.set_input(1, 2.0f);
    neuron2.set_input_weight(0, 0.5f);
    neuron2.set_input_weight(1, -0.3f);
    neuron2.set_bias(0.7f);

    // Enable threading on one neuron
    neuron1.enable_threads();
    neuron2.disable_threads();

    // Should still be equal since threading doesn't affect the stored values
    REQUIRE(neuron1 == neuron2);
    REQUIRE(neuron2 == neuron1);
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