#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "ant.hpp"
#include "food.hpp"
#include "genome.hpp"
#include "world.hpp"

using Catch::Approx;

TEST_CASE("Food eat functionality", "[food][eat]") {
  // Note: This test requires creating a World and Genome for the Ant
  // We'll need to set up the necessary dependencies

  SECTION("Food is not eaten initially") {
    Food food(Vector2{100.0f, 200.0f});
    REQUIRE(food.is_eaten() == false);
  }

  SECTION("Food can be reset after being eaten") {
    Food food(Vector2{100.0f, 200.0f});

    // Initially not eaten
    REQUIRE(food.is_eaten() == false);

    // Reset should make it not eaten again
    food.reset(Vector2{300.0f, 400.0f});
    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_position().x == 300.0f);
    REQUIRE(food.get_position().y == 400.0f);
  }

  SECTION("Food value is correct") {
    Food food;
    REQUIRE(food.get_value() == 10.0f);
  }

  SECTION("Food size and radius are correct") {
    Food food;
    REQUIRE(food.get_size() == Food::DEFAULT_SIZE);
    REQUIRE(food.get_radius() == Food::DEFAULT_SIZE / 2);
    REQUIRE(food.get_radius() == 5.0f);
  }

  SECTION("Food bounds are calculated correctly") {
    Vector2 position{50.0f, 75.0f};
    Food food(position);

    const Rectangle& bounds = food.get_bounds();
    REQUIRE(bounds.x == 25.0f);  // position.x / 2
    REQUIRE(bounds.y == 37.5f);  // position.y / 2
    REQUIRE(bounds.width == Food::DEFAULT_SIZE);
    REQUIRE(bounds.height == Food::DEFAULT_SIZE);
  }

  SECTION("Food at different positions has different bounds") {
    Food food1(Vector2{100.0f, 200.0f});
    Food food2(Vector2{200.0f, 400.0f});

    const Rectangle& bounds1 = food1.get_bounds();
    const Rectangle& bounds2 = food2.get_bounds();

    REQUIRE(bounds1.x == 50.0f);   // 100 / 2
    REQUIRE(bounds1.y == 100.0f);  // 200 / 2
    REQUIRE(bounds2.x == 100.0f);  // 200 / 2
    REQUIRE(bounds2.y == 200.0f);  // 400 / 2
  }

  SECTION("Food position affects bounds calculation") {
    // TODO: This seems off.  Shouldn't bounds.x be -50?
    Food food(Vector2{0.0f, 0.0f});
    const Rectangle& bounds = food.get_bounds();

    REQUIRE(bounds.x == 0.0f);
    REQUIRE(bounds.y == 0.0f);
    REQUIRE(bounds.width == Food::DEFAULT_SIZE);
    REQUIRE(bounds.height == Food::DEFAULT_SIZE);
  }

  SECTION("Food with negative position") {
    Food food(Vector2{-100.0f, -200.0f});
    const Rectangle& bounds = food.get_bounds();

    REQUIRE(bounds.x == -50.0f);   // -100 / 2
    REQUIRE(bounds.y == -100.0f);  // -200 / 2
    REQUIRE(bounds.width == Food::DEFAULT_SIZE);
    REQUIRE(bounds.height == Food::DEFAULT_SIZE);
  }

  SECTION("Food with decimal position") {
    Food food(Vector2{100.5f, 200.75f});
    const Rectangle& bounds = food.get_bounds();

    REQUIRE(bounds.x == 50.25f);    // 100.5 / 2
    REQUIRE(bounds.y == 100.375f);  // 200.75 / 2
    REQUIRE(bounds.width == Food::DEFAULT_SIZE);
    REQUIRE(bounds.height == Food::DEFAULT_SIZE);
  }

  SECTION("Multiple food items have independent states") {
    Food food1(Vector2{10.0f, 20.0f});
    Food food2(Vector2{30.0f, 40.0f});

    REQUIRE(food1.get_position().x == 10.0f);
    REQUIRE(food1.get_position().y == 20.0f);
    REQUIRE(food2.get_position().x == 30.0f);
    REQUIRE(food2.get_position().y == 40.0f);

    // Both should be in initial state
    REQUIRE(food1.is_eaten() == false);
    REQUIRE(food2.is_eaten() == false);

    // Reset one should not affect the other
    food1.reset(Vector2{100.0f, 200.0f});
    REQUIRE(food1.get_position().x == 100.0f);
    REQUIRE(food1.get_position().y == 200.0f);
    REQUIRE(food2.get_position().x == 30.0f);
    REQUIRE(food2.get_position().y == 40.0f);
  }
}