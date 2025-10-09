#include <catch2/catch_test_macros.hpp>

#include "texture_cache.hpp"
#include "world.hpp"

TEST_CASE("World initialization", "[world]") {
  SECTION("Default constructor") {
    TextureCache textureCache;
    World world(textureCache);
    
    // Check default bounds
    Rectangle bounds = world.get_bounds();
    REQUIRE(bounds.x == 0.0f);
    REQUIRE(bounds.y == 0.0f);
    REQUIRE(bounds.width == 1000.0f);
    REQUIRE(bounds.height == 1000.0f);
    
    // Check default spawn margin
    REQUIRE(world.get_spawn_margin() == 0.20f);
    
    // Check spawn bounds are calculated correctly
    Rectangle spawnBounds = world.get_spawn_bounds();
    float expectedMargin = bounds.width * 0.20f;  // 200.0f
    REQUIRE(spawnBounds.x == expectedMargin);
    REQUIRE(spawnBounds.y == expectedMargin);
    REQUIRE(spawnBounds.width == bounds.width - (2 * expectedMargin));
    REQUIRE(spawnBounds.height == bounds.height - (2 * expectedMargin));
    
    // Check population and resources exist
    REQUIRE_NOTHROW(world.get_population());
    REQUIRE_NOTHROW(world.get_resources());
  }

  SECTION("Spawn margin setter updates spawn bounds") {
    TextureCache textureCache;
    World world(textureCache);
    
    world.set_spawn_margin(0.10f);
    REQUIRE(world.get_spawn_margin() == 0.10f);
    
    Rectangle bounds = world.get_bounds();
    Rectangle spawnBounds = world.get_spawn_bounds();
    float expectedMargin = bounds.width * 0.10f;  // 100.0f
    
    REQUIRE(spawnBounds.x == expectedMargin);
    REQUIRE(spawnBounds.y == expectedMargin);
    REQUIRE(spawnBounds.width == bounds.width - (2 * expectedMargin));
    REQUIRE(spawnBounds.height == bounds.height - (2 * expectedMargin));
  }

  SECTION("Zero spawn margin") {
    TextureCache textureCache;
    World world(textureCache);
    
    world.set_spawn_margin(0.0f);
    REQUIRE(world.get_spawn_margin() == 0.0f);
    
    Rectangle bounds = world.get_bounds();
    Rectangle spawnBounds = world.get_spawn_bounds();
    
    REQUIRE(spawnBounds.x == 0.0f);
    REQUIRE(spawnBounds.y == 0.0f);
    REQUIRE(spawnBounds.width == bounds.width);
    REQUIRE(spawnBounds.height == bounds.height);
  }

  SECTION("Large spawn margin") {
    TextureCache textureCache;
    World world(textureCache);
    
    world.set_spawn_margin(0.45f);  // 45% margin each side = 90% total
    REQUIRE(world.get_spawn_margin() == 0.45f);
    
    Rectangle bounds = world.get_bounds();
    Rectangle spawnBounds = world.get_spawn_bounds();
    float expectedMargin = bounds.width * 0.45f;
    
    REQUIRE(spawnBounds.x == expectedMargin);
    REQUIRE(spawnBounds.y == expectedMargin);
    REQUIRE(spawnBounds.width == bounds.width - (2 * expectedMargin));
    REQUIRE(spawnBounds.height == bounds.height - (2 * expectedMargin));
  }
}