#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "neuron.hpp"
#include "random_generator.hpp"

using Catch::Approx;

TEST_CASE("Neuron serialization and deserialization", "[neuron][serialization]") {
  RandomGenerator generator;
  SECTION("Empty neuron serialization") {
    Neuron neuron;
    auto json = neuron.to_json();

    REQUIRE(json.contains("bias"));
    REQUIRE(json.contains("weights"));
    REQUIRE(json.contains("inputs"));
    REQUIRE(json.contains("value"));

    REQUIRE(json["bias"] == 0.0f);
    REQUIRE(json["weights"].is_array());
    REQUIRE(json["weights"].size() == 0);
    REQUIRE(json["inputs"].is_array());
    REQUIRE(json["inputs"].size() == 0);
    REQUIRE(json["value"] == 0.0f);
  }

  SECTION("Configured neuron serialization") {
    Neuron neuron;
    neuron.set_input_count(3);
    neuron.set_input(0, 1.0f);
    neuron.set_input(1, 2.0f);
    neuron.set_input(2, 3.0f);
    neuron.set_input_weight(0, 0.5f);
    neuron.set_input_weight(1, -0.3f);
    neuron.set_input_weight(2, 0.8f);
    neuron.set_bias(0.7f);

    // Trigger calculation to set _value
    neuron.get_output();

    auto json = neuron.to_json();

    REQUIRE(json["bias"] == 0.7f);
    REQUIRE(json["weights"].size() == 3);
    REQUIRE(json["weights"][0] == 0.5f);
    REQUIRE(json["weights"][1] == -0.3f);
    REQUIRE(json["weights"][2] == 0.8f);
    REQUIRE(json["inputs"].size() == 3);
    REQUIRE(json["inputs"][0] == 1.0f);
    REQUIRE(json["inputs"][1] == 2.0f);
    REQUIRE(json["inputs"][2] == 3.0f);
    // Note: _value will be the result of activation function, so we just check it's a number
    REQUIRE(json["value"].is_number());
  }

  SECTION("Neuron deserialization from JSON") {
    nlohmann::json json;
    json["bias"] = 0.5f;
    json["weights"] = {0.1f, -0.2f, 0.3f};
    json["inputs"] = {1.0f, 2.0f, 3.0f};
    json["value"] = 0.8f;

    Neuron neuron(json);

    REQUIRE(neuron.get_bias() == 0.5f);
    REQUIRE(neuron.get_input_count() == 3);
    REQUIRE(neuron.get_input(0) == 1.0f);
    REQUIRE(neuron.get_input(1) == 2.0f);
    REQUIRE(neuron.get_input(2) == 3.0f);
    REQUIRE(neuron.get_input_weight(0) == 0.1f);
    REQUIRE(neuron.get_input_weight(1) == -0.2f);
    REQUIRE(neuron.get_input_weight(2) == 0.3f);
  }

  SECTION("Round-trip serialization and deserialization") {
    Neuron original;
    original.set_input_count(4);
    original.set_input(0, 0.1f);
    original.set_input(1, 0.2f);
    original.set_input(2, 0.3f);
    original.set_input(3, 0.4f);
    original.set_input_weight(0, 0.5f);
    original.set_input_weight(1, 0.6f);
    original.set_input_weight(2, 0.7f);
    original.set_input_weight(3, 0.8f);
    original.set_bias(0.9f);

    // Trigger calculation
    original.get_output();

    auto json = original.to_json();
    Neuron deserialized = Neuron(json);
    REQUIRE(deserialized == original);
  }

  SECTION("Serialization with randomized neuron") {
    Neuron neuron;
    neuron.set_input_count(5);
    neuron.randomize(generator);

    // Set some inputs to known values for testing
    neuron.set_input(0, 1.0f);
    neuron.set_input(1, 2.0f);
    neuron.set_input(2, 3.0f);
    neuron.set_input(3, 4.0f);
    neuron.set_input(4, 5.0f);

    auto json = neuron.to_json();

    REQUIRE(json["bias"].is_number());
    REQUIRE(json["weights"].size() == 5);
    REQUIRE(json["inputs"].size() == 5);
    REQUIRE(json["value"].is_number());

    // All weights should be in the randomization range [-1, 1]
    for (const auto& weight : json["weights"]) {
      REQUIRE(weight.get<float>() >= -3.0f);
      REQUIRE(weight.get<float>() <= 3.0f);
    }

    // Bias should also be in the randomization range
    REQUIRE(json["bias"].get<float>() >= -1.0f);
    REQUIRE(json["bias"].get<float>() <= 1.0f);
  }

  SECTION("Edge case: zero inputs and weights") {
    Neuron neuron;
    neuron.set_input_count(0);

    auto json = neuron.to_json();
    REQUIRE(json["weights"].size() == 0);
    REQUIRE(json["inputs"].size() == 0);

    Neuron deserialized = Neuron(json);
    REQUIRE(deserialized.get_input_count() == 0);
  }

  SECTION("Edge case: large number of inputs") {
    const size_t large_count = 100;
    Neuron neuron;
    neuron.set_input_count(large_count);

    // Set some values
    for (size_t i = 0; i < large_count; ++i) {
      neuron.set_input(i, static_cast<float>(i));
      neuron.set_input_weight(i, static_cast<float>(i) * 0.1f);
    }

    auto json = neuron.to_json();
    REQUIRE(json["weights"].size() == large_count);
    REQUIRE(json["inputs"].size() == large_count);

    Neuron deserialized = Neuron(json);
    REQUIRE(deserialized == neuron);
  }
}