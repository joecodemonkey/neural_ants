#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "neuron.hpp"

using Catch::Approx;

TEST_CASE("Neuron copy and move operations", "[neuron]") {
  SECTION("Copy constructor") {
    Neuron original;
    original.set_input_count(2);
    original.set_input(0, 1.0f);
    original.set_input_weight(0, 0.5f);
    original.set_bias(0.2f);

    Neuron copy(original);
    REQUIRE(copy.get_input_count() == 2);
    REQUIRE(copy.get_input(0) == 1.0f);
    REQUIRE(copy.get_input_weight(0) == 0.5f);
    REQUIRE(copy.get_bias() == 0.2f);
  }

  SECTION("Copy assignment") {
    Neuron original;
    original.set_input_count(2);
    original.set_input(0, 1.0f);
    original.set_input_weight(0, 0.5f);
    original.set_bias(0.2f);

    Neuron copy;
    copy = original;
    REQUIRE(copy.get_input_count() == 2);
    REQUIRE(copy.get_input(0) == 1.0f);
    REQUIRE(copy.get_input_weight(0) == 0.5f);
    REQUIRE(copy.get_bias() == 0.2f);
  }

  SECTION("Move constructor") {
    Neuron original;
    original.set_input_count(2);
    original.set_input(0, 1.0f);
    original.set_input_weight(0, 0.5f);
    original.set_bias(0.2f);

    Neuron moved(std::move(original));
    REQUIRE(moved.get_input_count() == 2);
    REQUIRE(moved.get_input(0) == 1.0f);
    REQUIRE(moved.get_input_weight(0) == 0.5f);
    REQUIRE(moved.get_bias() == 0.2f);
  }

  SECTION("Move assignment") {
    Neuron original;
    original.set_input_count(2);
    original.set_input(0, 1.0f);
    original.set_input_weight(0, 0.5f);
    original.set_bias(0.2f);

    Neuron moved;
    moved = std::move(original);
    REQUIRE(moved.get_input_count() == 2);
    REQUIRE(moved.get_input(0) == 1.0f);
    REQUIRE(moved.get_input_weight(0) == 0.5f);
    REQUIRE(moved.get_bias() == 0.2f);
  }
}