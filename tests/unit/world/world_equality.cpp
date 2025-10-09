#include <catch2/catch_test_macros.hpp>

#include "texture_cache.hpp"
#include "world.hpp"

TEST_CASE("World equality operator", "[world]") {
  SECTION("Default constructed worlds are equal") {
    TextureCache textureCache;
    World world1(textureCache);
    World world2(textureCache);

    REQUIRE(world1 == world2);
    REQUIRE(world2 == world1);
  }

  SECTION("Identical configured worlds are equal") {
    TextureCache textureCache;
    World world1(textureCache);
    World world2(textureCache);

    world1.set_spawn_margin(0.15f);
    world2.set_spawn_margin(0.15f);

    REQUIRE(world1 == world2);
    REQUIRE(world2 == world1);
  }

  SECTION("Copied worlds are equal") {
    TextureCache textureCache;
    World original(textureCache);
    original.set_spawn_margin(0.25f);

    World copy(original);

    REQUIRE(original == copy);
    REQUIRE(copy == original);
  }

  SECTION("Assigned worlds are equal") {
    TextureCache textureCache;
    World original(textureCache);
    World assigned(textureCache);

    original.set_spawn_margin(0.30f);
    assigned = original;

    REQUIRE(original == assigned);
    REQUIRE(assigned == original);
  }

  SECTION("Worlds with different spawn margins are not equal") {
    TextureCache textureCache;
    World world1(textureCache);
    World world2(textureCache);

    world1.set_spawn_margin(0.10f);
    world2.set_spawn_margin(0.20f);

    REQUIRE_FALSE(world1 == world2);
    REQUIRE_FALSE(world2 == world1);
  }

  SECTION("Self equality") {
    TextureCache textureCache;
    World world(textureCache);
    world.set_spawn_margin(0.35f);

    REQUIRE(world == world);
  }

  SECTION("Equality includes population state") {
    TextureCache textureCache;
    World world1(textureCache);
    World world2(textureCache);

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
    TextureCache textureCache;
    World world1(textureCache);
    World world2(textureCache);

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