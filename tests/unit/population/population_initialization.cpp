#include <catch2/catch_test_macros.hpp>

#include "population.hpp"
#include "world.hpp"

TEST_CASE("Population initialization", "[population]") {
  World world;

  SECTION("Default constructor") {
    Population population(world);
    
    REQUIRE(population.get_size() == 1);  // DEFAULT_POPULATION_SIZE
    REQUIRE(population.get_texture_path().empty());
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

  SECTION("Texture path management") {
    Population population(world);
    
    REQUIRE(population.get_texture_path().empty());
    
    population.set_texture_path("/path/to/texture.png");
    REQUIRE(population.get_texture_path() == "/path/to/texture.png");
    
    population.set_texture_path("");
    REQUIRE(population.get_texture_path().empty());
  }
}