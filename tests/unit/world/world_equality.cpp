#include <catch2/catch_test_macros.hpp>

#include "world.hpp"

TEST_CASE("World equality operator", "[world]") {
  SECTION("Default constructed worlds are equal") {
    World world1;
    World world2;
    
    REQUIRE(world1 == world2);
    REQUIRE(world2 == world1);
  }

  SECTION("Identical configured worlds are equal") {
    World world1;
    World world2;
    
    world1.set_spawn_margin(0.15f);
    world2.set_spawn_margin(0.15f);
    
    REQUIRE(world1 == world2);
    REQUIRE(world2 == world1);
  }

  SECTION("Copied worlds are equal") {
    World original;
    original.set_spawn_margin(0.25f);
    
    World copy(original);
    
    REQUIRE(original == copy);
    REQUIRE(copy == original);
  }

  SECTION("Assigned worlds are equal") {
    World original;
    World assigned;
    
    original.set_spawn_margin(0.30f);
    assigned = original;
    
    REQUIRE(original == assigned);
    REQUIRE(assigned == original);
  }

  SECTION("Worlds with different spawn margins are not equal") {
    World world1;
    World world2;
    
    world1.set_spawn_margin(0.10f);
    world2.set_spawn_margin(0.20f);
    
    REQUIRE_FALSE(world1 == world2);
    REQUIRE_FALSE(world2 == world1);
  }

  SECTION("Self equality") {
    World world;
    world.set_spawn_margin(0.35f);
    
    REQUIRE(world == world);
  }

  SECTION("Equality includes population state") {
    World world1;
    World world2;
    
    // Initially equal
    REQUIRE(world1 == world2);
    
    // Modify population size in one world
    world1.get_population().set_size(5);
    world2.get_population().set_size(3);
    
    REQUIRE_FALSE(world1 == world2);
    
    // Make them equal again
    world2.get_population().set_size(5);
    REQUIRE(world1 == world2);
  }

  SECTION("Equality includes resources state") {
    World world1;
    World world2;
    
    // Initially equal
    REQUIRE(world1 == world2);
    
    // Modify resource count in one world
    world1.get_resources().set_food_count(50);
    world2.get_resources().set_food_count(100);
    
    REQUIRE_FALSE(world1 == world2);
    
    // Make them equal again
    world2.get_resources().set_food_count(50);
    REQUIRE(world1 == world2);
  }
}