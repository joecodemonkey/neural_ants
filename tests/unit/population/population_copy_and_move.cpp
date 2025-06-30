#include <catch2/catch_test_macros.hpp>

#include "population.hpp"
#include "world.hpp"

TEST_CASE("Population copy and move operations", "[population]") {
  World world;

  SECTION("Copy constructor") {
    Population original(world);
    original.set_size(5);
    original.set_texture_path("/test/path.png");

    Population copy(original);

    REQUIRE(copy == original);
  }

  SECTION("Copy assignment operator") {
    Population original(world);
    Population assigned(world);

    original.set_size(15);
    original.set_texture_path("/another/path.png");

    assigned = original;

    REQUIRE(assigned == original);
  }

  SECTION("Move constructor") {
    Population original(world);
    original.set_size(25);
    original.set_texture_path("/move/path.png");

    Population moved(std::move(original));

    REQUIRE(moved.get_size() == 25);
    REQUIRE(moved.get_texture_path() == "/move/path.png");

    // Original should be in valid but unspecified state
    REQUIRE(original.get_size() == 0);
    REQUIRE(original.get_texture_path().empty());
  }

  SECTION("Move assignment operator") {
    Population original(world);
    Population moveAssigned(world);

    original.set_size(35);
    original.set_texture_path("/move_assign/path.png");

    moveAssigned = std::move(original);

    REQUIRE(moveAssigned.get_size() == 35);
    REQUIRE(moveAssigned.get_texture_path() == "/move_assign/path.png");

    REQUIRE(original.get_size() == 0);
    REQUIRE(original.get_texture_path().empty());
  }

  SECTION("Self assignment protection") {
    Population population(world);
    population.set_size(42);
    population.set_texture_path("/self/path.png");

    Population original_copy = population;

    // Test self copy assignment
    population = population;
    REQUIRE(population == original_copy);

    // Test self move assignment
    population = std::move(population);
    REQUIRE(population == original_copy);
  }
}