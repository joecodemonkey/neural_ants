#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "texture_cache.hpp"
#include "world.hpp"

TEST_CASE("World JSON serialization", "[world]") {
  SECTION("Basic to_json functionality") {
    TextureCache textureCache;
    World world(textureCache);
    world.set_spawn_margin(0.15f);

    nlohmann::json j = world.to_json();

    REQUIRE(j.contains("bounds"));
    REQUIRE(j.contains("spawn_bounds"));
    REQUIRE(j.contains("spawn_margin"));
    REQUIRE(j.contains("resources"));
    REQUIRE(j.contains("population"));

    REQUIRE(j["spawn_margin"] == 0.15f);
  }

  SECTION("JSON round trip") {
    TextureCache textureCache;
    World original(textureCache);
    original.set_spawn_margin(0.25f);
    original.get_population().set_size(7);
    original.get_resources().set_food_count(150);

    nlohmann::json j = original.to_json();
    World restored(j, textureCache);

    REQUIRE(restored == original);
    REQUIRE(restored.get_spawn_margin() == 0.25f);
    REQUIRE(restored.get_population().get_size() == 7);
    REQUIRE(restored.get_resources().get_food_count() == 150);
  }

  SECTION("JSON constructor with valid data") {
    TextureCache textureCache;
    nlohmann::json j;
    j["bounds"] = {{"x", 0.0f}, {"y", 0.0f}, {"width", 1000.0f}, {"height", 1000.0f}};
    j["spawn_bounds"] = {{"x", 200.0f}, {"y", 200.0f}, {"width", 600.0f}, {"height", 600.0f}};
    j["spawn_margin"] = 0.20f;
    j["resources"] = {{"food_count", 100}, {"food", nlohmann::json::array()}};
    j["population"] = {
        {"size", 1},
        {"ants", nlohmann::json::array()},
        {"pangenome", {{"genomes", nlohmann::json::array()}, {"top_cycle_index", 0}}}};

    REQUIRE_NOTHROW(World(j, textureCache));

    World world(j, textureCache);
    REQUIRE(world.get_spawn_margin() == 0.20f);
  }

  SECTION("JSON constructor with missing fields") {
    TextureCache textureCache;
    nlohmann::json j;
    j["bounds"] = {{"x", 0.0f}, {"y", 0.0f}, {"width", 1000.0f}, {"height", 1000.0f}};
    // Missing spawn_bounds, spawn_margin, resources, population

    REQUIRE_THROWS(World(j, textureCache));
  }

  SECTION("JSON constructor with invalid spawn margin") {
    TextureCache textureCache;
    nlohmann::json j;
    j["bounds"] = {{"x", 0.0f}, {"y", 0.0f}, {"width", 1000.0f}, {"height", 1000.0f}};
    j["spawn_bounds"] = {{"x", 200.0f}, {"y", 200.0f}, {"width", 600.0f}, {"height", 600.0f}};
    j["spawn_margin"] = "invalid";  // Should be float
    j["resources"] = {{"food_count", 100}, {"food", nlohmann::json::array()}};
    j["population"] = {
        {"size", 1},
        {"ants", nlohmann::json::array()},
        {"pangenome", {{"genomes", nlohmann::json::array()}, {"top_cycle_index", 0}}}};

    REQUIRE_THROWS(World(j, textureCache));
  }

  SECTION("Bounds serialization preserves values") {
    TextureCache textureCache;
    World world(textureCache);

    nlohmann::json j = world.to_json();

    // Check bounds are serialized correctly
    REQUIRE(j["bounds"]["x"] == 0.0f);
    REQUIRE(j["bounds"]["y"] == 0.0f);
    REQUIRE(j["bounds"]["width"] == 1000.0f);
    REQUIRE(j["bounds"]["height"] == 1000.0f);
  }

  SECTION("Spawn bounds serialization preserves values") {
    TextureCache textureCache;
    World world(textureCache);
    world.set_spawn_margin(0.10f);

    nlohmann::json j = world.to_json();

    // Check spawn bounds are serialized correctly
    REQUIRE(j["spawn_bounds"]["x"] == 100.0f);       // 10% of 1000
    REQUIRE(j["spawn_bounds"]["y"] == 100.0f);       // 10% of 1000
    REQUIRE(j["spawn_bounds"]["width"] == 800.0f);   // 1000 - 2*100
    REQUIRE(j["spawn_bounds"]["height"] == 800.0f);  // 1000 - 2*100
  }
}