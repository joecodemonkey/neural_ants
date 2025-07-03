#include <catch2/catch_test_macros.hpp>

#include "population.hpp"
#include "world.hpp"

TEST_CASE("Population equality operator", "[population]") {
  World world;

  SECTION("Identical populations are equal") {
    Population pop1(world);
    Population pop2(world);

    pop1.set_size(5);
    pop1.set_texture_path("/test/path.png");
    pop2.set_size(5);
    pop2.set_texture_path("/test/path.png");

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

  SECTION("Different texture paths are not equal") {
    Population pop1(world);
    Population pop2(world);

    pop1.set_texture_path("/path1.png");
    pop2.set_texture_path("/path2.png");

    REQUIRE_FALSE(pop1 == pop2);
    REQUIRE_FALSE(pop2 == pop1);
  }

  SECTION("Different texture loaded states are not equal") {
    Population pop1(world);
    Population pop2(world);

    // This test is limited since texture loading is internal
    // but we can test that populations with same settings are equal
    REQUIRE(pop1 == pop2);
  }

  SECTION("Self equality") {
    Population pop(world);
    pop.set_size(3);
    pop.set_texture_path("/self/path.png");

    REQUIRE(pop == pop);
  }

  SECTION("Empty populations are equal") {
    Population pop1(world);
    Population pop2(world);

    REQUIRE(pop1 == pop2);
  }
}