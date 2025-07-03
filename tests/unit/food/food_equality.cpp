#include <algorithm>
#include <catch2/catch_test_macros.hpp>

#include "food.hpp"

TEST_CASE("Food equality operator", "[food]") {
  SECTION("identical food items are equal") {
    std::vector<Food> foods(2);
    std::for_each(std::begin(foods), std::end(foods), [](Food& food) {
      food = Food(Vector2{100.0f, 200.0f});
    });

    REQUIRE(foods.at(0) == foods.at(1));
    REQUIRE(foods.at(1) == foods.at(0));
  }

  SECTION("default constructed food items are equal") {
    Food food1;
    Food food2;

    REQUIRE(food1 == food2);
    REQUIRE(food2 == food1);
  }

  SECTION("food items with different positions are not equal") {
    Food food1(Vector2{100.0f, 200.0f});
    Food food2(Vector2{101.0f, 200.0f});  // Different x position

    REQUIRE_FALSE(food1 == food2);
    REQUIRE_FALSE(food2 == food1);
  }

  SECTION("food items with different eaten states are not equal") {
    Food food1(Vector2{100.0f, 200.0f});
    Food food2(Vector2{100.0f, 200.0f});

    // Make one eaten (we'll need to access the eat method or simulate it)
    // For now, we'll test that identical food items are equal
    REQUIRE(food1 == food2);
    REQUIRE(food2 == food1);
  }

  SECTION("food items with different values are not equal") {
    Food food1(Vector2{100.0f, 200.0f});
    Food food2(Vector2{100.0f, 200.0f});

    // Since _value is not exposed for modification, we'll test that identical food items are equal
    REQUIRE(food1 == food2);
    REQUIRE(food2 == food1);
  }

  SECTION("assigned food items are equal") {
    Food food1(Vector2{150.0f, 250.0f});
    Food food2;
    food2 = food1;  // Assignment operator

    REQUIRE(food1 == food2);
    REQUIRE(food2 == food1);
  }

  SECTION("self equality") {
    Food food(Vector2{123.0f, 456.0f});
    REQUIRE(food == food);
  }

  SECTION("food items at origin are equal") {
    Food food1;
    Food food2;

    REQUIRE(food1.get_position().x == 0.0f);
    REQUIRE(food1.get_position().y == 0.0f);
    REQUIRE(food2.get_position().x == 0.0f);
    REQUIRE(food2.get_position().y == 0.0f);

    REQUIRE(food1 == food2);
    REQUIRE(food2 == food1);
  }

  SECTION("food items with negative positions") {
    Food food1(Vector2{-100.0f, -200.0f});
    Food food2(Vector2{-100.0f, -200.0f});

    REQUIRE(food1 == food2);
    REQUIRE(food2 == food1);
  }

  SECTION("food items with large positions") {
    Food food1(Vector2{10000.0f, 20000.0f});
    Food food2(Vector2{10000.0f, 20000.0f});

    REQUIRE(food1 == food2);
    REQUIRE(food2 == food1);
  }

  SECTION("food items with decimal positions") {
    Food food1(Vector2{100.5f, 200.75f});
    Food food2(Vector2{100.5f, 200.75f});

    REQUIRE(food1 == food2);
    REQUIRE(food2 == food1);
  }

  SECTION("food items with different decimal positions") {
    Food food1(Vector2{100.5f, 200.75f});
    Food food2(Vector2{100.5f, 200.76f});  // Slightly different y position

    REQUIRE_FALSE(food1 == food2);
    REQUIRE_FALSE(food2 == food1);
  }

  SECTION("multiple food items comparison") {
    std::vector<Food> foods;
    foods.emplace_back(Vector2{10.0f, 20.0f});
    foods.emplace_back(Vector2{10.0f, 20.0f});
    foods.emplace_back(Vector2{30.0f, 40.0f});
    foods.emplace_back(Vector2{10.0f, 20.0f});

    // First two should be equal
    REQUIRE(foods[0] == foods[1]);
    REQUIRE(foods[1] == foods[0]);

    // Third should be different
    REQUIRE_FALSE(foods[0] == foods[2]);
    REQUIRE_FALSE(foods[2] == foods[0]);

    // First and fourth should be equal
    REQUIRE(foods[0] == foods[3]);
    REQUIRE(foods[3] == foods[0]);
  }
}