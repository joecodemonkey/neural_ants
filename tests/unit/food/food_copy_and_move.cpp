#include <catch2/catch_test_macros.hpp>

#include "food.hpp"

TEST_CASE("Food copy and move operations", "[food]") {
  SECTION("Assignment operator") {
    Food food1(Vector2{100.0f, 200.0f});
    Food food2;

    food2 = food1;  // Assignment operator

    REQUIRE(food2 == food1);
    REQUIRE(food2.get_position().x == 100.0f);
    REQUIRE(food2.get_position().y == 200.0f);
    REQUIRE(food2.get_value() == 10.0f);
    REQUIRE(food2.is_eaten() == false);
  }

  SECTION("Self assignment") {
    Food food(Vector2{150.0f, 250.0f});
    food = food;  // Self assignment should be safe

    REQUIRE(food.get_position().x == 150.0f);
    REQUIRE(food.get_position().y == 250.0f);
    REQUIRE(food.get_value() == 10.0f);
    REQUIRE(food.is_eaten() == false);
  }

  SECTION("Assignment preserves all properties") {
    Food food1(Vector2{123.0f, 456.0f});
    Food food2(Vector2{789.0f, 012.0f});

    // Verify they're different initially
    REQUIRE_FALSE(food1 == food2);

    food2 = food1;

    // Now they should be equal
    REQUIRE(food2 == food1);
    REQUIRE(food2.get_position().x == 123.0f);
    REQUIRE(food2.get_position().y == 456.0f);
    REQUIRE(food2.get_value() == food1.get_value());
    REQUIRE(food2.is_eaten() == food1.is_eaten());
  }

  SECTION("Assignment with different positions") {
    Food food1(Vector2{10.0f, 20.0f});
    Food food2(Vector2{30.0f, 40.0f});

    REQUIRE_FALSE(food1 == food2);

    food2 = food1;

    REQUIRE(food2 == food1);
    REQUIRE(food2.get_position().x == 10.0f);
    REQUIRE(food2.get_position().y == 20.0f);
  }

  SECTION("Assignment with negative positions") {
    Food food1(Vector2{-100.0f, -200.0f});
    Food food2;

    food2 = food1;

    REQUIRE(food2 == food1);
    REQUIRE(food2.get_position().x == -100.0f);
    REQUIRE(food2.get_position().y == -200.0f);
  }

  SECTION("Assignment with decimal positions") {
    Food food1(Vector2{100.5f, 200.75f});
    Food food2;

    food2 = food1;

    REQUIRE(food2 == food1);
    REQUIRE(food2.get_position().x == 100.5f);
    REQUIRE(food2.get_position().y == 200.75f);
  }

  SECTION("Assignment with large positions") {
    Food food1(Vector2{10000.0f, 20000.0f});
    Food food2;

    food2 = food1;

    REQUIRE(food2 == food1);
    REQUIRE(food2.get_position().x == 10000.0f);
    REQUIRE(food2.get_position().y == 20000.0f);
  }

  SECTION("Assignment at origin") {
    Food food1;  // Default constructor creates food at origin
    Food food2(Vector2{100.0f, 200.0f});

    food2 = food1;

    REQUIRE(food2 == food1);
    REQUIRE(food2.get_position().x == 0.0f);
    REQUIRE(food2.get_position().y == 0.0f);
  }

  SECTION("Multiple assignments") {
    Food food1(Vector2{10.0f, 20.0f});
    Food food2(Vector2{30.0f, 40.0f});
    Food food3(Vector2{50.0f, 60.0f});

    food3 = food2 = food1;  // Chain assignment

    REQUIRE(food3 == food1);
    REQUIRE(food2 == food1);
    REQUIRE(food3.get_position().x == 10.0f);
    REQUIRE(food3.get_position().y == 20.0f);
  }

  SECTION("Assignment preserves bounds") {
    Food food1(Vector2{100.0f, 200.0f});
    Food food2;

    food2 = food1;

    const Rectangle& bounds1 = food1.get_bounds();
    const Rectangle& bounds2 = food2.get_bounds();

    REQUIRE(bounds2.x == bounds1.x);
    REQUIRE(bounds2.y == bounds1.y);
    REQUIRE(bounds2.width == bounds1.width);
    REQUIRE(bounds2.height == bounds1.height);
  }

  SECTION("Assignment preserves radius") {
    Food food1(Vector2{100.0f, 200.0f});
    Food food2;

    food2 = food1;

    REQUIRE(food2.get_radius() == food1.get_radius());
    REQUIRE(food2.get_radius() == Food::DEFAULT_SIZE / 2);
  }
}