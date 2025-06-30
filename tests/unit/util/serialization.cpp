#include "util/serialization.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Rectangle Serialization", "[serialization]") {
  SECTION("rectangle_to_json") {
    Rectangle rect{10.5f, 20.3f, 100.0f, 200.0f};
    auto json = Util::rectangle_to_json(rect);

    REQUIRE(json["x"] == 10.5f);
    REQUIRE(json["y"] == 20.3f);
    REQUIRE(json["width"] == 100.0f);
    REQUIRE(json["height"] == 200.0f);
    REQUIRE(json.size() == 4);
  }

  SECTION("rectangle_from_json") {
    nlohmann::json json = {{"x", 15.7f}, {"y", 25.9f}, {"width", 150.0f}, {"height", 300.0f}};

    auto rect = Util::rectangle_from_json(json);

    REQUIRE(rect.x == 15.7f);
    REQUIRE(rect.y == 25.9f);
    REQUIRE(rect.width == 150.0f);
    REQUIRE(rect.height == 300.0f);
  }

  SECTION("rectangle_roundtrip") {
    Rectangle original{5.5f, 10.5f, 50.0f, 75.0f};
    auto json = Util::rectangle_to_json(original);
    auto reconstructed = Util::rectangle_from_json(json);

    REQUIRE(reconstructed.x == original.x);
    REQUIRE(reconstructed.y == original.y);
    REQUIRE(reconstructed.width == original.width);
    REQUIRE(reconstructed.height == original.height);
  }

  SECTION("rectangle_from_json_missing_fields") {
    nlohmann::json json = {
        {"x", 10.0f}, {"y", 20.0f}  // Missing width and height
    };

    REQUIRE_THROWS_AS(Util::rectangle_from_json(json), nlohmann::json::out_of_range);
  }
}

TEST_CASE("Vector2 Serialization", "[serialization]") {
  SECTION("vector2_to_json") {
    Vector2 vec{30.5f, 40.7f};
    auto json = Util::vector2_to_json(vec);

    REQUIRE(json["x"] == 30.5f);
    REQUIRE(json["y"] == 40.7f);
    REQUIRE(json.size() == 2);
  }

  SECTION("vector2_from_json") {
    nlohmann::json json = {{"x", 35.2f}, {"y", 45.8f}};

    auto vec = Util::vector2_from_json(json);

    REQUIRE(vec.x == 35.2f);
    REQUIRE(vec.y == 45.8f);
  }

  SECTION("vector2_roundtrip") {
    Vector2 original{12.3f, 34.5f};
    auto json = Util::vector2_to_json(original);
    auto reconstructed = Util::vector2_from_json(json);

    REQUIRE(reconstructed.x == original.x);
    REQUIRE(reconstructed.y == original.y);
  }

  SECTION("vector2_from_json_missing_fields") {
    nlohmann::json json = {
        {"x", 10.0f}  // Missing y
    };

    REQUIRE_THROWS_AS(Util::vector2_from_json(json), nlohmann::json::out_of_range);
  }

  SECTION("vector2_from_json_wrong_type") {
    nlohmann::json json = {{"x", "not_a_number"}, {"y", 20.0f}};

    REQUIRE_THROWS_AS(Util::vector2_from_json(json), nlohmann::json::type_error);
  }
}

TEST_CASE("Camera2D Serialization", "[serialization]") {
  SECTION("camera2d_to_json") {
    Camera2D camera{
        {100.0f, 200.0f},  // target
        {50.0f, 75.0f},    // offset
        45.0f,             // rotation
        2.5f               // zoom
    };

    auto json = Util::camera2d_to_json(camera);

    REQUIRE(json["target"]["x"] == 50.0f);
    REQUIRE(json["target"]["y"] == 75.0f);
    REQUIRE(json["offset"]["x"] == 100.0f);
    REQUIRE(json["offset"]["y"] == 200.0f);
    REQUIRE(json["rotation"] == 45.0f);
    REQUIRE(json["zoom"] == 2.5f);
    REQUIRE(json.size() == 4);
  }

  SECTION("camera2d_from_json") {
    nlohmann::json json = {{"target", {{"x", 150.0f}, {"y", 250.0f}}},
                           {"offset", {{"x", 60.0f}, {"y", 80.0f}}},
                           {"rotation", 90.0f},
                           {"zoom", 1.5f}};

    auto camera = Util::camera2d_from_json(json);

    REQUIRE(camera.target.x == 150.0f);
    REQUIRE(camera.target.y == 250.0f);
    REQUIRE(camera.offset.x == 60.0f);
    REQUIRE(camera.offset.y == 80.0f);
    REQUIRE(camera.rotation == 90.0f);
    REQUIRE(camera.zoom == 1.5f);
  }

  SECTION("camera2d_roundtrip") {
    Camera2D original{
        {25.5f, 35.7f},  // target
        {10.2f, 15.8f},  // offset
        30.0f,           // rotation
        1.8f             // zoom
    };

    auto json = Util::camera2d_to_json(original);
    auto reconstructed = Util::camera2d_from_json(json);

    REQUIRE(reconstructed.target.x == original.target.x);
    REQUIRE(reconstructed.target.y == original.target.y);
    REQUIRE(reconstructed.offset.x == original.offset.x);
    REQUIRE(reconstructed.offset.y == original.offset.y);
    REQUIRE(reconstructed.rotation == original.rotation);
    REQUIRE(reconstructed.zoom == original.zoom);
  }

  SECTION("camera2d_from_json_missing_fields") {
    nlohmann::json json = {
        {"target", {{"x", 100.0f}, {"y", 200.0f}}}, {"offset", {{"x", 50.0f}, {"y", 75.0f}}}
        // Missing rotation and zoom
    };

    REQUIRE_THROWS_AS(Util::camera2d_from_json(json), nlohmann::json::out_of_range);
  }

  SECTION("camera2d_from_json_invalid_target") {
    nlohmann::json json = {{"target", {{"x", 100.0f}}},  // Missing y in target
                           {"offset", {{"x", 50.0f}, {"y", 75.0f}}},
                           {"rotation", 0.0f},
                           {"zoom", 1.0f}};

    REQUIRE_THROWS_AS(Util::camera2d_from_json(json), nlohmann::json::out_of_range);
  }

  SECTION("camera2d_edge_cases") {
    // Test with zero values
    Camera2D zero_camera{
        {0.0f, 0.0f},  // target
        {0.0f, 0.0f},  // offset
        0.0f,          // rotation
        0.0f           // zoom
    };

    auto json = Util::camera2d_to_json(zero_camera);
    auto reconstructed = Util::camera2d_from_json(json);

    REQUIRE(reconstructed.target.x == 0.0f);
    REQUIRE(reconstructed.target.y == 0.0f);
    REQUIRE(reconstructed.offset.x == 0.0f);
    REQUIRE(reconstructed.offset.y == 0.0f);
    REQUIRE(reconstructed.rotation == 0.0f);
    REQUIRE(reconstructed.zoom == 0.0f);

    // Test with negative values
    Camera2D negative_camera{
        {-100.0f, -200.0f},  // target
        {-50.0f, -75.0f},    // offset
        -45.0f,              // rotation
        -1.0f                // zoom
    };

    json = Util::camera2d_to_json(negative_camera);
    reconstructed = Util::camera2d_from_json(json);

    REQUIRE(reconstructed.target.x == -50.0f);
    REQUIRE(reconstructed.target.y == -75.0f);
    REQUIRE(reconstructed.offset.x == -100.0f);
    REQUIRE(reconstructed.offset.y == -200.0f);
    REQUIRE(reconstructed.rotation == -45.0f);
    REQUIRE(reconstructed.zoom == -1.0f);
  }
}