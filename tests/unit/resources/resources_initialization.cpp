#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include "raylib.h"
#include "resources.hpp"
#include "texture_cache.hpp"
#include "world.hpp"

using Catch::Approx;

TEST_CASE("Resources initialization and basic operations", "[resources]") {
  SECTION("Default initialization") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);

    REQUIRE(resources.get_food_count() == 200);  // DEFAULT_COUNT
  }

  SECTION("Copy constructor") {
    TextureCache textureCache;
    World world(textureCache);
    Resources original(world);
    original.set_food_count(75);

    Resources copy(original);

    REQUIRE(copy.get_food_count() == 75);
    REQUIRE(copy == original);
  }

  SECTION("Assignment operator") {
    TextureCache textureCache;
    World world(textureCache);
    Resources original(world);
    original.set_food_count(60);

    Resources assigned(world);
    assigned = original;

    REQUIRE(assigned.get_food_count() == 60);
    REQUIRE(assigned == original);
  }

  SECTION("Self assignment") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);
    resources.set_food_count(80);

    resources = resources;  // Self assignment

    REQUIRE(resources.get_food_count() == 80);
  }

  SECTION("Setting and getting food count") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);

    REQUIRE(resources.get_food_count() == 200);

    resources.set_food_count(150);
    REQUIRE(resources.get_food_count() == 150);

    resources.set_food_count(0);
    REQUIRE(resources.get_food_count() == 0);

    resources.set_food_count(200);
    REQUIRE(resources.get_food_count() == 200);
  }

  SECTION("Food count edge cases") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);

    // Test negative values (should work as int)
    resources.set_food_count(-10);
    REQUIRE(resources.get_food_count() == -10);

    // Test large values
    resources.set_food_count(10000);
    REQUIRE(resources.get_food_count() == 10000);
  }
}