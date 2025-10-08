#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include "food.hpp"
#include "food_test_helper.hpp"
#include "raylib.h"

using Catch::Approx;

TEST_CASE("Food initialization and basic operations", "[food]") {
  SECTION("Default initialization") {
    Food food(get_mock_texture_cache());

    REQUIRE(food.get_position().x == 0.0f);
    REQUIRE(food.get_position().y == 0.0f);
    REQUIRE(food.get_value() == 500.0f);
    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_radius() == Food::RADIUS);
  }

  SECTION("Position-based initialization") {
    Vector2 position{100.0f, 200.0f};
    Food food(position, get_mock_texture_cache());

    REQUIRE(food.get_position().x == 100.0f);
    REQUIRE(food.get_position().y == 200.0f);
    REQUIRE(food.get_value() == 500.0f);
    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_radius() == Food::RADIUS);
  }

  SECTION("Getting bounds") {
    Vector2 position{50.0f, 75.0f};
    Food food(position, get_mock_texture_cache());

    const Rectangle& bounds = food.get_bounds();
    REQUIRE(bounds.x == 42.0f);  // position.x - (16/2)
    REQUIRE(bounds.y == 67.0f);  // position.y - (16/2)
    REQUIRE(bounds.width == Food::TEXTURE_WIDTH);
    REQUIRE(bounds.height == Food::TEXTURE_WIDTH);
  }

  SECTION("Getting radius") {
    Food food(get_mock_texture_cache());
    REQUIRE(food.get_radius() == Food::RADIUS);
    REQUIRE(food.get_radius() == 8.0f);  // 16/2
  }

  SECTION("Getting value") {
    Food food(get_mock_texture_cache());
    REQUIRE(food.get_value() == 500.0f);
  }

  SECTION("Getting position") {
    Vector2 position{123.0f, 456.0f};
    Food food(position, get_mock_texture_cache());

    const Vector2& retrieved_position = food.get_position();
    REQUIRE(retrieved_position.x == 123.0f);
    REQUIRE(retrieved_position.y == 456.0f);
  }

  SECTION("Checking eaten status") {
    Food food(get_mock_texture_cache());
    REQUIRE(food.is_eaten() == false);
  }

  SECTION("Reset functionality") {
    Food food(Vector2{500.0f, 20.0f}, get_mock_texture_cache());

    // Initially not eaten
    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_position().x == 500.0f);
    REQUIRE(food.get_position().y == 20.0f);

    // Reset to new position
    Vector2 new_position{30.0f, 40.0f};
    food.reset(new_position);

    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_position().x == 30.0f);
    REQUIRE(food.get_position().y == 40.0f);
  }

  SECTION("Reset after being eaten") {
    Food food(get_mock_texture_cache());

    // Simulate being eaten (we'll test this more thoroughly in eat tests)
    // For now, just test that reset works regardless of eaten state
    Vector2 new_position{100.0f, 200.0f};
    food.reset(new_position);

    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_position().x == 100.0f);
    REQUIRE(food.get_position().y == 200.0f);
  }

  SECTION("Multiple food instances") {
    Food food1(Vector2{500.0f, 20.0f}, get_mock_texture_cache());
    Food food2(Vector2{30.0f, 40.0f}, get_mock_texture_cache());

    REQUIRE(food1.get_position().x == 500.0f);
    REQUIRE(food1.get_position().y == 20.0f);
    REQUIRE(food2.get_position().x == 30.0f);
    REQUIRE(food2.get_position().y == 40.0f);

    // Both should have same default values
    REQUIRE(food1.get_value() == food2.get_value());
    REQUIRE(food1.is_eaten() == food2.is_eaten());
  }
}