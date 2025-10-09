#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "food.hpp"
#include "food_test_helper.hpp"

using Catch::Approx;

TEST_CASE("Food serialization and deserialization", "[food][serialization]") {
  SECTION("Default food serialization") {
    Food food(get_mock_texture_cache());
    auto json = food.to_json();

    REQUIRE(json.contains("value"));
    REQUIRE(json.contains("position"));
    REQUIRE(json.contains("eaten"));
    REQUIRE(json.contains("textureIndex"));

    REQUIRE(json["value"] == 500.0f);
    REQUIRE(json["position"].contains("x"));
    REQUIRE(json["position"].contains("y"));
    REQUIRE(json["position"]["x"] == 0.0f);
    REQUIRE(json["position"]["y"] == 0.0f);
    REQUIRE(json["eaten"] == false);
  }

  SECTION("Positioned food serialization") {
    Food food(Vector2{100.0f, 200.0f}, get_mock_texture_cache());
    auto json = food.to_json();

    REQUIRE(json["value"] == 500.0f);
    REQUIRE(json["position"]["x"] == 100.0f);
    REQUIRE(json["position"]["y"] == 200.0f);
    REQUIRE(json["eaten"] == false);
  }

  SECTION("Food deserialization from JSON") {
    nlohmann::json json;
    json["value"] = 15.0f;
    json["position"]["x"] = 150.0f;
    json["position"]["y"] = 250.0f;
    json["eaten"] = false;
    json["textureIndex"] = 0;

    Food food(json, get_mock_texture_cache());

    REQUIRE(food.get_value() == 15.0f);
    REQUIRE(food.get_position().x == 150.0f);
    REQUIRE(food.get_position().y == 250.0f);
    REQUIRE(food.is_eaten() == false);
  }

  SECTION("Round-trip serialization and deserialization") {
    Food original(Vector2{123.0f, 456.0f}, get_mock_texture_cache());

    auto json = original.to_json();
    Food deserialized(json, get_mock_texture_cache());

    REQUIRE(deserialized == original);
  }

  SECTION("Serialization with eaten food") {
    Food food(Vector2{50.0f, 75.0f}, get_mock_texture_cache());

    // Note: We can't directly test eaten state since eat() requires an Ant
    // But we can test that the serialization structure is correct
    auto json = food.to_json();

    REQUIRE(json["eaten"] == false);
    REQUIRE(json["position"]["x"] == 50.0f);
    REQUIRE(json["position"]["y"] == 75.0f);
  }

  SECTION("Edge case: food at origin") {
    Food food(get_mock_texture_cache());
    auto json = food.to_json();

    REQUIRE(json["position"]["x"] == 0.0f);
    REQUIRE(json["position"]["y"] == 0.0f);

    Food deserialized = Food(json, get_mock_texture_cache());
    REQUIRE(deserialized == food);
  }

  SECTION("Edge case: food with negative position") {
    Food food(Vector2{-100.0f, -200.0f}, get_mock_texture_cache());
    auto json = food.to_json();

    REQUIRE(json["position"]["x"] == -100.0f);
    REQUIRE(json["position"]["y"] == -200.0f);

    Food deserialized = Food(json, get_mock_texture_cache());
    REQUIRE(deserialized == food);
  }

  SECTION("Edge case: food with large position") {
    Food food(Vector2{10000.0f, 20000.0f}, get_mock_texture_cache());
    auto json = food.to_json();

    REQUIRE(json["position"]["x"] == 10000.0f);
    REQUIRE(json["position"]["y"] == 20000.0f);

    Food deserialized = Food(json, get_mock_texture_cache());
    REQUIRE(deserialized == food);
  }

  SECTION("Edge case: food with decimal position") {
    Food food(Vector2{100.5f, 200.75f}, get_mock_texture_cache());
    auto json = food.to_json();

    REQUIRE(json["position"]["x"] == 100.5f);
    REQUIRE(json["position"]["y"] == 200.75f);

    Food deserialized = Food(json, get_mock_texture_cache());
    REQUIRE(deserialized == food);
  }
}