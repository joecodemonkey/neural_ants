#include <catch2/catch_test_macros.hpp>

#include "population.hpp"
#include "world.hpp"

TEST_CASE("Population initialization", "[population]") {
  World world;

  SECTION("Default constructor") {
    Population population(world);

    REQUIRE(population.get_size() == 1);  // DEFAULT_POPULATION_SIZE
  }

  SECTION("Set and get size") {
    Population population(world);

    population.set_size(10);
    REQUIRE(population.get_size() == 10);

    population.set_size(0);
    REQUIRE(population.get_size() == 0);

    population.set_size(100);
    REQUIRE(population.get_size() == 100);
  }
}