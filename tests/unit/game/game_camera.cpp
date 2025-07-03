#include <catch2/catch_test_macros.hpp>

#include "game.hpp"
#include "raylib.h"

TEST_CASE("Game camera operations", "[game]") {
  SECTION("Camera getter and setter") {
    Game game;

    // Test camera getter (const)
    const auto& constCamera = game.get_camera();

    REQUIRE(constCamera.offset.x == 0.0f);
    REQUIRE(constCamera.offset.y == 0.0f);
    REQUIRE(constCamera.target.x == 0.0f);
    REQUIRE(constCamera.target.y == 0.0f);
    REQUIRE(constCamera.rotation == 0.0f);
    REQUIRE(constCamera.zoom == 1.0f);

    // Test camera getter (non-const)
    auto& camera = game.get_camera();
    REQUIRE(camera.offset.x == 0.0f);
    REQUIRE(camera.offset.y == 0.0f);
    REQUIRE(camera.target.x == 0.0f);
    REQUIRE(camera.target.y == 0.0f);
    REQUIRE(camera.rotation == 0.0f);
    REQUIRE(camera.zoom == 1.0f);

    // Test camera setter
    Camera2D newCamera = {
        .offset = {100.0f, 200.0f}, .target = {300.0f, 400.0f}, .rotation = 45.0f, .zoom = 2.0f};

    game.set_camera(newCamera);

    // Verify the camera was updated
    auto updatedCamera = game.get_camera();
    REQUIRE(updatedCamera.offset.x == 100.0f);
    REQUIRE(updatedCamera.offset.y == 200.0f);
    REQUIRE(updatedCamera.target.x == 300.0f);
    REQUIRE(updatedCamera.target.y == 400.0f);
    REQUIRE(updatedCamera.rotation == 45.0f);
    REQUIRE(updatedCamera.zoom == 2.0f);
  }

  SECTION("Camera modification through reference") {
    Game game;

    // Modify camera through non-const reference
    auto& camera = game.get_camera();
    camera.offset.x = 50.0f;
    camera.offset.y = 75.0f;
    camera.zoom = 1.5f;

    // Verify changes are reflected
    auto updatedCamera = game.get_camera();
    REQUIRE(updatedCamera.offset.x == 50.0f);
    REQUIRE(updatedCamera.offset.y == 75.0f);
    REQUIRE(updatedCamera.zoom == 1.5f);
  }
}