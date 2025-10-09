#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "ant.hpp"
#include "food.hpp"
#include "food_test_helper.hpp"
#include "genome.hpp"
#include "world.hpp"

using Catch::Approx;

TEST_CASE("Food eat functionality", "[food][eat]") {
  // Note: This test requires creating a World and Genome for the Ant
  // We'll need to set up the necessary dependencies

  SECTION("Food is not eaten initially") {
    Food food(Vector2{100.0f, 200.0f}, get_mock_texture_cache());
    REQUIRE(food.is_eaten() == false);
  }

  SECTION("Food can be reset after being eaten") {
    Food food(Vector2{100.0f, 200.0f}, get_mock_texture_cache());

    // Initially not eaten
    REQUIRE(food.is_eaten() == false);

    // Reset should make it not eaten again
    food.reset(Vector2{300.0f, 400.0f});
    REQUIRE(food.is_eaten() == false);
    REQUIRE(food.get_position().x == 300.0f);
    REQUIRE(food.get_position().y == 400.0f);
  }

  SECTION("Food value is correct") {
    Food food(get_mock_texture_cache());
    REQUIRE(food.get_value() == 500.0f);
  }

  SECTION("Food size and radius are correct") {
    Food food(get_mock_texture_cache());
    REQUIRE(food.get_radius() == Food::RADIUS);
    REQUIRE(food.get_radius() == 8.0f);  // 16/2
  }

  SECTION("Food bounds are calculated correctly") {
    Vector2 position{50.0f, 75.0f};
    Food food(position, get_mock_texture_cache());

    const Rectangle& bounds = food.get_bounds();
    REQUIRE(bounds.x == 42.0f);  // position.x - (16/2)
    REQUIRE(bounds.y == 67.0f);  // position.y - (16/2)
    REQUIRE(bounds.width == Food::TEXTURE_WIDTH);
    REQUIRE(bounds.height == Food::TEXTURE_WIDTH);
  }

  SECTION("Food at different positions has different bounds") {
    Food food1(Vector2{100.0f, 200.0f}, get_mock_texture_cache());
    Food food2(Vector2{200.0f, 400.0f}, get_mock_texture_cache());

    const Rectangle& bounds1 = food1.get_bounds();
    const Rectangle& bounds2 = food2.get_bounds();

    REQUIRE(bounds1.x == 92.0f);   // 100 - 8
    REQUIRE(bounds1.y == 192.0f);  // 200 - 8
    REQUIRE(bounds2.x == 192.0f);  // 200 - 8
    REQUIRE(bounds2.y == 392.0f);  // 400 - 8
  }

  SECTION("Food position affects bounds calculation") {
    Food food(Vector2{0.0f, 0.0f}, get_mock_texture_cache());
    const Rectangle& bounds = food.get_bounds();

    REQUIRE(bounds.x == -8.0f);  // 0 - 8
    REQUIRE(bounds.y == -8.0f);  // 0 - 8
    REQUIRE(bounds.width == Food::TEXTURE_WIDTH);
    REQUIRE(bounds.height == Food::TEXTURE_WIDTH);
  }

  SECTION("Food with negative position") {
    Food food(Vector2{-100.0f, -200.0f}, get_mock_texture_cache());
    const Rectangle& bounds = food.get_bounds();

    REQUIRE(bounds.x == -108.0f);   // -100 - 8
    REQUIRE(bounds.y == -208.0f);  // -200 - 8
    REQUIRE(bounds.width == Food::TEXTURE_WIDTH);
    REQUIRE(bounds.height == Food::TEXTURE_WIDTH);
  }

  SECTION("Food with decimal position") {
    Food food(Vector2{100.5f, 200.75f}, get_mock_texture_cache());
    const Rectangle& bounds = food.get_bounds();

    REQUIRE(bounds.x == 92.5f);    // 100.5 - 8
    REQUIRE(bounds.y == 192.75f);  // 200.75 - 8
    REQUIRE(bounds.width == Food::TEXTURE_WIDTH);
    REQUIRE(bounds.height == Food::TEXTURE_WIDTH);
  }

  SECTION("Multiple food items have independent states") {
    Food food1(Vector2{500.0f, 20.0f}, get_mock_texture_cache());
    Food food2(Vector2{30.0f, 40.0f}, get_mock_texture_cache());

    REQUIRE(food1.get_position().x == 500.0f);
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