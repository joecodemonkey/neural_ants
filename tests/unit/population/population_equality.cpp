#include <catch2/catch_test_macros.hpp>

#include "population.hpp"
#include "texture_cache.hpp"
#include "world.hpp"

TEST_CASE("Population equality operator", "[population]") {
  TextureCache textureCache;
  World world(textureCache);

  SECTION("Identical populations are equal") {
    Population pop1(world);
    Population pop2(world);

    pop1.set_size(5);
    pop2.set_size(5);

    REQUIRE(pop1 == pop2);
    REQUIRE(pop2 == pop1);
  }

  SECTION("Different sizes are not equal") {
    Population pop1(world);
    Population pop2(world);

    pop1.set_size(5);
    pop2.set_size(10);

    REQUIRE_FALSE(pop1 == pop2);
    REQUIRE_FALSE(pop2 == pop1);
  }

  SECTION("Self equality") {
    Population pop(world);
    pop.set_size(3);

    REQUIRE(pop == pop);
  }

  SECTION("Empty populations are equal") {
    Population pop1(world);
    Population pop2(world);

    REQUIRE(pop1 == pop2);
  }
}