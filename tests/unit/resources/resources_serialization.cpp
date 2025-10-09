#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "resources.hpp"
#include "texture_cache.hpp"
#include "world.hpp"

using Catch::Approx;

TEST_CASE("Resources serialization and deserialization", "[resources][serialization]") {
  SECTION("Default resources serialization") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);
    auto json = resources.to_json();

    REQUIRE(json.contains("food_count"));
    REQUIRE(json.contains("food"));

    REQUIRE(json["food_count"] == 200);  // DEFAULT_COUNT
    REQUIRE(json["food"].is_array());
    REQUIRE(json["food"].size() == 0);  // Initially no food items
  }

  SECTION("Resources with custom food count serialization") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);
    resources.set_food_count(75);

    auto json = resources.to_json();

    REQUIRE(json["food_count"] == 75);
    REQUIRE(json["food"].is_array());
  }

  SECTION("Round-trip serialization and deserialization") {
    TextureCache textureCache;
    World world(textureCache);
    Resources original(world);
    original.set_food_count(60);

    auto json = original.to_json();
    Resources deserialized(json, world);

    REQUIRE(deserialized.get_food_count() == original.get_food_count());
    REQUIRE(deserialized == original);
  }

  SECTION("Edge case: zero food count") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);
    resources.set_food_count(0);

    auto json = resources.to_json();

    REQUIRE(json["food_count"] == 0);
    REQUIRE(json["food"].is_array());

    Resources deserialized = Resources(json, world);
    REQUIRE(deserialized.get_food_count() == 0);
  }

  SECTION("Edge case: negative food count") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);
    resources.set_food_count(-10);

    auto json = resources.to_json();

    REQUIRE(json["food_count"] == -10);
    REQUIRE(json["food"].is_array());

    Resources deserialized = Resources(json, world);
    REQUIRE(deserialized.get_food_count() == -10);
  }

  SECTION("Serialization structure validation") {
    TextureCache textureCache;
    World world(textureCache);
    Resources resources(world);
    auto json = resources.to_json();

    // Check that the JSON has the expected structure
    REQUIRE(json.is_object());
    REQUIRE(json.size() == 2);  // food_count and food
    REQUIRE(json["food_count"].is_number());
    REQUIRE(json["food"].is_array());
  }
}