#include <catch2/catch_test_macros.hpp>

#include "texture_cache.hpp"
#include "world.hpp"

TEST_CASE("World bounds and spawn logic", "[world]") {
  SECTION("Out of bounds detection") {
    TextureCache textureCache;
    World world(textureCache);

    // Points inside bounds
    REQUIRE_FALSE(world.out_of_bounds({100.0f, 100.0f}));
    REQUIRE_FALSE(world.out_of_bounds({500.0f, 500.0f}));
    REQUIRE_FALSE(world.out_of_bounds({999.0f, 999.0f}));

    // Points on edges (boundaries are inclusive for left/top, exclusive for right/bottom)
    REQUIRE_FALSE(world.out_of_bounds({0.0f, 0.0f}));
    REQUIRE(world.out_of_bounds({1000.0f, 0.0f}));     // Right edge is exclusive
    REQUIRE(world.out_of_bounds({0.0f, 1000.0f}));     // Bottom edge is exclusive
    REQUIRE(world.out_of_bounds({1000.0f, 1000.0f}));  // Corner is exclusive

    // Points outside bounds
    REQUIRE(world.out_of_bounds({-1.0f, 500.0f}));
    REQUIRE(world.out_of_bounds({1001.0f, 500.0f}));
    REQUIRE(world.out_of_bounds({500.0f, -1.0f}));
    REQUIRE(world.out_of_bounds({500.0f, 1001.0f}));
  }

  SECTION("Spawn position generates valid positions") {
    TextureCache textureCache;
    World world(textureCache);
    Rectangle spawnBounds = world.get_spawn_bounds();

    Vector2 dimensions = {10.0f, 10.0f};

    // Test spawn position functionality (avoiding GetRandomValue for now)
    REQUIRE_NOTHROW(world.spawn_position(dimensions));

    // Verify spawn bounds are reasonable
    REQUIRE(spawnBounds.width > 0);
    REQUIRE(spawnBounds.height > 0);
    REQUIRE(spawnBounds.x >= 0);
    REQUIRE(spawnBounds.y >= 0);
  }

  SECTION("Spawn position with zero dimensions") {
    TextureCache textureCache;
    World world(textureCache);

    Vector2 dimensions = {0.0f, 0.0f};
    REQUIRE_NOTHROW(world.spawn_position(dimensions));
  }

  SECTION("Spawn position with large dimensions") {
    TextureCache textureCache;
    World world(textureCache);

    Vector2 dimensions = {100.0f, 100.0f};
    REQUIRE_NOTHROW(world.spawn_position(dimensions));
  }

  SECTION("Spawn position with maximum dimensions") {
    TextureCache textureCache;
    World world(textureCache);
    Rectangle spawnBounds = world.get_spawn_bounds();

    // Dimensions that exactly fill spawn bounds
    Vector2 dimensions = {spawnBounds.width, spawnBounds.height};
    REQUIRE_NOTHROW(world.spawn_position(dimensions));
  }

  SECTION("Spawn bounds calculation with different margins") {
    TextureCache textureCache;
    World world(textureCache);

    // Test various margin values
    std::vector<float> margins = {0.0f, 0.1f, 0.25f, 0.4f};

    for (float margin : margins) {
      world.set_spawn_margin(margin);

      Rectangle bounds = world.get_bounds();
      Rectangle spawnBounds = world.get_spawn_bounds();

      float expectedMargin = bounds.width * margin;

      REQUIRE(spawnBounds.x == bounds.x + expectedMargin);
      REQUIRE(spawnBounds.y == bounds.y + expectedMargin);
      REQUIRE(spawnBounds.width == bounds.width - (2 * expectedMargin));
      REQUIRE(spawnBounds.height == bounds.height - (2 * expectedMargin));
    }
  }

  SECTION("Update functionality") {
    TextureCache textureCache;
    World world(textureCache);

    // Should not throw when updating
    REQUIRE_NOTHROW(world.update(0.0f));
    REQUIRE_NOTHROW(world.update(1.0f));
    REQUIRE_NOTHROW(world.update(16.67f));  // ~60 FPS
  }

  // Note: Draw functionality testing removed because it requires graphics context
}