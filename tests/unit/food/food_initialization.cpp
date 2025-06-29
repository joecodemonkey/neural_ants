#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>

#include "food.hpp"
#include "raylib.h"

using Catch::Approx;

TEST_CASE("Food initialization and basic operations", "[food]") {
  SECTION("Default initialization") {
    Food food;

    REQUIRE(food.get_position().x == 0.0f);
    REQUIRE(food.get_position().y == 0.0f);
    REQUIRE(food.get_value() == 10.0f);
    REQUIRE(food.get_size() == Food::DEFAULT_SIZE);
    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_radius() == Food::DEFAULT_SIZE / 2);
  }

  SECTION("Position-based initialization") {
    Vector2 position{100.0f, 200.0f};
    Food food(position);

    REQUIRE(food.get_position().x == 100.0f);
    REQUIRE(food.get_position().y == 200.0f);
    REQUIRE(food.get_value() == 10.0f);
    REQUIRE(food.get_size() == Food::DEFAULT_SIZE);
    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_radius() == Food::DEFAULT_SIZE / 2);
  }

  SECTION("Getting bounds") {
    Vector2 position{50.0f, 75.0f};
    Food food(position);

    const Rectangle& bounds = food.get_bounds();
    REQUIRE(bounds.x == 25.0f);  // position.x / 2
    REQUIRE(bounds.y == 37.5f);  // position.y / 2
    REQUIRE(bounds.width == Food::DEFAULT_SIZE);
    REQUIRE(bounds.height == Food::DEFAULT_SIZE);
  }

  SECTION("Getting radius") {
    Food food;
    REQUIRE(food.get_radius() == Food::DEFAULT_SIZE / 2);
    REQUIRE(food.get_radius() == 5.0f);
  }

  SECTION("Getting size") {
    Food food;
    REQUIRE(food.get_size() == Food::DEFAULT_SIZE);
    REQUIRE(food.get_size() == 10.0f);
  }

  SECTION("Getting value") {
    Food food;
    REQUIRE(food.get_value() == 10.0f);
  }

  SECTION("Getting position") {
    Vector2 position{123.0f, 456.0f};
    Food food(position);

    const Vector2& retrieved_position = food.get_position();
    REQUIRE(retrieved_position.x == 123.0f);
    REQUIRE(retrieved_position.y == 456.0f);
  }

  SECTION("Checking eaten status") {
    Food food;
    REQUIRE(food.is_eaten() == false);
  }

  SECTION("Reset functionality") {
    Food food(Vector2{10.0f, 20.0f});

    // Initially not eaten
    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_position().x == 10.0f);
    REQUIRE(food.get_position().y == 20.0f);

    // Reset to new position
    Vector2 new_position{30.0f, 40.0f};
    food.reset(new_position);

    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_position().x == 30.0f);
    REQUIRE(food.get_position().y == 40.0f);
  }

  SECTION("Reset after being eaten") {
    Food food;

    // Simulate being eaten (we'll test this more thoroughly in eat tests)
    // For now, just test that reset works regardless of eaten state
    Vector2 new_position{100.0f, 200.0f};
    food.reset(new_position);

    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_position().x == 100.0f);
    REQUIRE(food.get_position().y == 200.0f);
  }

  SECTION("Multiple food instances") {
    Food food1(Vector2{10.0f, 20.0f});
    Food food2(Vector2{30.0f, 40.0f});

    REQUIRE(food1.get_position().x == 10.0f);
    REQUIRE(food1.get_position().y == 20.0f);
    REQUIRE(food2.get_position().x == 30.0f);
    REQUIRE(food2.get_position().y == 40.0f);

    // Both should have same default values
    REQUIRE(food1.get_value() == food2.get_value());
    REQUIRE(food1.get_size() == food2.get_size());
    REQUIRE(food1.is_eaten() == food2.is_eaten());
  }
}