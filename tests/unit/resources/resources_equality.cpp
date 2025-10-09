#include <algorithm>
#include <catch2/catch_test_macros.hpp>

#include "resources.hpp"
#include "texture_cache.hpp"
#include "world.hpp"

TEST_CASE("Resources equality operator", "[resources]") {
  SECTION("identical resources are equal") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources1(world);
    Resources resources2(world);

    resources1.set_food_count(50);
    resources2.set_food_count(50);

    REQUIRE(resources1 == resources2);
    REQUIRE(resources2 == resources1);
  }

  SECTION("default constructed resources are equal") {
    TextureCache textureCache;
    World world1(textureCache);
    World world2(textureCache);
    Resources resources1(world1);
    Resources resources2(world2);

    REQUIRE(resources1 == resources2);
    REQUIRE(resources2 == resources1);
  }

  SECTION("resources with different food counts are not equal") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources1(world);
    Resources resources2(world);

    resources1.set_food_count(50);
    resources2.set_food_count(75);

    REQUIRE_FALSE(resources1 == resources2);
    REQUIRE_FALSE(resources2 == resources1);
  }

  SECTION("assigned resources are equal") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources1(world);
    Resources resources2(world);

    resources1.set_food_count(100);
    resources2 = resources1;  // Assignment operator

    REQUIRE(resources1 == resources2);
    REQUIRE(resources2 == resources1);
  }

  SECTION("self equality") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);
    resources.set_food_count(123);

    REQUIRE(resources == resources);
  }

  SECTION("resources with zero food count") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources1(world);
    Resources resources2(world);

    resources1.set_food_count(0);
    resources2.set_food_count(0);

    REQUIRE(resources1 == resources2);
    REQUIRE(resources2 == resources1);
  }

  SECTION("resources with negative food count") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources1(world);
    Resources resources2(world);

    resources1.set_food_count(-10);
    resources2.set_food_count(-10);

    REQUIRE(resources1 == resources2);
    REQUIRE(resources2 == resources1);
  }

  SECTION("copy constructed resources are equal") {
    TextureCache textureCache;
    World world(textureCache);
    Resources original(world);
    original.set_food_count(75);

    Resources copy(original);

    REQUIRE(original == copy);
    REQUIRE(copy == original);
  }

  SECTION("resources after modification") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources1(world);
    Resources resources2(world);

    // Initially equal (both have default food count of 200)
    REQUIRE(resources1 == resources2);

    // Modify one to a different value
    resources1.set_food_count(150);

    // Now different
    REQUIRE_FALSE(resources1 == resources2);
    REQUIRE_FALSE(resources2 == resources1);

    // Make them equal again
    resources2.set_food_count(150);
    REQUIRE(resources1 == resources2);
    REQUIRE(resources2 == resources1);
  }
}