#include <catch2/catch_test_macros.hpp>

#include "world.hpp"

TEST_CASE("World copy and move operations", "[world]") {
  SECTION("Copy constructor") {
    World original;
    original.set_spawn_margin(0.15f);
    
    World copy(original);
    
    REQUIRE(copy == original);
    REQUIRE(copy.get_spawn_margin() == 0.15f);
    
    // Verify bounds are identical
    Rectangle originalBounds = original.get_bounds();
    Rectangle copyBounds = copy.get_bounds();
    REQUIRE(copyBounds.x == originalBounds.x);
    REQUIRE(copyBounds.y == originalBounds.y);
    REQUIRE(copyBounds.width == originalBounds.width);
    REQUIRE(copyBounds.height == originalBounds.height);
    
    // Verify spawn bounds are identical
    Rectangle originalSpawnBounds = original.get_spawn_bounds();
    Rectangle copySpawnBounds = copy.get_spawn_bounds();
    REQUIRE(copySpawnBounds.x == originalSpawnBounds.x);
    REQUIRE(copySpawnBounds.y == originalSpawnBounds.y);
    REQUIRE(copySpawnBounds.width == originalSpawnBounds.width);
    REQUIRE(copySpawnBounds.height == originalSpawnBounds.height);
  }

  SECTION("Copy assignment operator") {
    World original;
    World assigned;
    
    original.set_spawn_margin(0.25f);
    
    assigned = original;
    
    REQUIRE(assigned == original);
    REQUIRE(assigned.get_spawn_margin() == 0.25f);
  }

  SECTION("Move constructor") {
    World original;
    original.set_spawn_margin(0.30f);
    
    Rectangle originalBounds = original.get_bounds();
    Rectangle originalSpawnBounds = original.get_spawn_bounds();
    
    World moved(std::move(original));
    
    REQUIRE(moved.get_spawn_margin() == 0.30f);
    
    // Verify moved object has correct bounds
    Rectangle movedBounds = moved.get_bounds();
    REQUIRE(movedBounds.x == originalBounds.x);
    REQUIRE(movedBounds.y == originalBounds.y);
    REQUIRE(movedBounds.width == originalBounds.width);
    REQUIRE(movedBounds.height == originalBounds.height);
    
    Rectangle movedSpawnBounds = moved.get_spawn_bounds();
    REQUIRE(movedSpawnBounds.x == originalSpawnBounds.x);
    REQUIRE(movedSpawnBounds.y == originalSpawnBounds.y);
    REQUIRE(movedSpawnBounds.width == originalSpawnBounds.width);
    REQUIRE(movedSpawnBounds.height == originalSpawnBounds.height);
    
    // Original should be in valid but reset state
    Rectangle resetBounds = original.get_bounds();
    REQUIRE(resetBounds.x == 0.0f);
    REQUIRE(resetBounds.y == 0.0f);
    REQUIRE(resetBounds.width == 1000.0f);
    REQUIRE(resetBounds.height == 1000.0f);
    REQUIRE(original.get_spawn_margin() == 0.20f);  // DEFAULT_SPAWN_MARGIN
  }

  SECTION("Move assignment operator") {
    World original;
    World moveAssigned;
    
    original.set_spawn_margin(0.35f);
    
    Rectangle originalBounds = original.get_bounds();
    Rectangle originalSpawnBounds = original.get_spawn_bounds();
    
    moveAssigned = std::move(original);
    
    REQUIRE(moveAssigned.get_spawn_margin() == 0.35f);
    
    // Verify moved object has correct bounds
    Rectangle movedBounds = moveAssigned.get_bounds();
    REQUIRE(movedBounds.x == originalBounds.x);
    REQUIRE(movedBounds.y == originalBounds.y);
    REQUIRE(movedBounds.width == originalBounds.width);
    REQUIRE(movedBounds.height == originalBounds.height);
    
    // Original should be in valid but reset state
    REQUIRE(original.get_spawn_margin() == 0.20f);  // DEFAULT_SPAWN_MARGIN
  }

  SECTION("Self assignment protection") {
    World world;
    world.set_spawn_margin(0.42f);
    
    World originalCopy = world;
    
    // Test self copy assignment
    world = world;
    REQUIRE(world == originalCopy);
    
    // Test self move assignment
    world = std::move(world);
    REQUIRE(world == originalCopy);
  }

  SECTION("Independent modification after copy") {
    World original;
    original.set_spawn_margin(0.20f);
    
    World copy(original);
    
    // Modify original
    original.set_spawn_margin(0.30f);
    
    // Copy should remain unchanged
    REQUIRE(copy.get_spawn_margin() == 0.20f);
    REQUIRE(original.get_spawn_margin() == 0.30f);
    REQUIRE_FALSE(copy == original);
  }
}