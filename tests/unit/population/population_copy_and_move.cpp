#include <catch2/catch_test_macros.hpp>

#include "population.hpp"
#include "world.hpp"

TEST_CASE("Population copy and move operations", "[population]") {
  World world;

  SECTION("Copy constructor") {
    Population original(world);
    original.set_size(5);

    Population copy(original);

    REQUIRE(copy == original);
  }

  SECTION("Copy assignment operator") {
    Population original(world);
    Population assigned(world);

    original.set_size(15);

    assigned = original;

    REQUIRE(assigned == original);
  }

  SECTION("Self assignment protection") {
    Population population(world);
    population.set_size(42);

    Population original_copy = population;

    // Test self copy assignment
    population = population;
    REQUIRE(population == original_copy);

    // Test self move assignment
    population = std::move(population);
    REQUIRE(population == original_copy);
  }
}