#include "surroundings.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Surroundings default constructor", "[surroundings]") {
  Surroundings surroundings;
  REQUIRE(surroundings.get_width() == 0);
  REQUIRE(surroundings.get_height() == 0);
  REQUIRE_FALSE(surroundings.changed());
}

TEST_CASE("Surroundings set dimensions", "[surroundings]") {
  Surroundings surroundings;
  surroundings.set_dimensions(5, 3);
  REQUIRE(surroundings.get_width() == 5);
  REQUIRE(surroundings.get_height() == 3);
  REQUIRE(surroundings.changed());

  // Verify all squares default to EMPTY
  const auto& encoded = surroundings.get_encoded_surroundings();
  for (const auto& value : encoded) {
    REQUIRE(value == 0.0f);  // EMPTY should encode to 0.0f
  }
}

TEST_CASE("Surroundings set type", "[surroundings]") {
  Surroundings surroundings;
  surroundings.set_dimensions(3, 3);

  // Test setting different types
  surroundings.set_type(0, 0, Surroundings::FOOD);
  surroundings.set_type(1, 1, Surroundings::WALL);
  surroundings.set_type(2, 2, Surroundings::EMPTY);

  // Get encoded surroundings and verify values
  const auto& encoded = surroundings.get_encoded_surroundings();
  REQUIRE(encoded[0] == 1.0f);   // FOOD
  REQUIRE(encoded[4] == -1.0f);  // WALL
  REQUIRE(encoded[8] == 0.0f);   // EMPTY
}

TEST_CASE("Surroundings out of bounds set type", "[surroundings]") {
  Surroundings surroundings;
  surroundings.set_dimensions(2, 2);

  // Test setting type out of bounds
  REQUIRE_THROWS_AS(surroundings.set_type(2, 0, Surroundings::FOOD), std::out_of_range);
  REQUIRE_THROWS_AS(surroundings.set_type(0, 2, Surroundings::FOOD), std::out_of_range);
  REQUIRE_THROWS_AS(surroundings.set_type(-1, 0, Surroundings::FOOD), std::out_of_range);
  REQUIRE_THROWS_AS(surroundings.set_type(0, -1, Surroundings::FOOD), std::out_of_range);
}

TEST_CASE("Surroundings get dimensions", "[surroundings]") {
  Surroundings surroundings;
  surroundings.set_dimensions(4, 6);
  auto dimensions = surroundings.get_dimensions();
  REQUIRE(dimensions.x == 4);
  REQUIRE(dimensions.y == 6);
}

TEST_CASE("Surroundings changed flag", "[surroundings]") {
  Surroundings surroundings;

  // Initially not changed
  REQUIRE_FALSE(surroundings.changed());

  // Set dimensions should mark as changed
  surroundings.set_dimensions(3, 3);
  REQUIRE(surroundings.changed());

  // Getting encoded surroundings should reset changed flag
  surroundings.get_encoded_surroundings();
  REQUIRE_FALSE(surroundings.changed());

  // Setting a type should mark as changed
  surroundings.set_type(1, 1, Surroundings::FOOD);
  REQUIRE(surroundings.changed());
}

TEST_CASE("Surroundings encode type", "[surroundings]") {
  Surroundings surroundings;

  // Test all possible type encodings
  REQUIRE(surroundings.get_encoded_surroundings().size() == 0);  // Empty initially

  surroundings.set_dimensions(1, 1);
  surroundings.set_type(0, 0, Surroundings::FOOD);
  REQUIRE(surroundings.get_encoded_surroundings()[0] == 1.0f);

  surroundings.set_type(0, 0, Surroundings::EMPTY);
  REQUIRE(surroundings.get_encoded_surroundings()[0] == 0.0f);

  surroundings.set_type(0, 0, Surroundings::WALL);
  REQUIRE(surroundings.get_encoded_surroundings()[0] == -1.0f);
}

TEST_CASE("Surroundings copy constructor", "[surroundings]") {
  // Set up original surroundings
  Surroundings surroundings;
  surroundings.set_dimensions(2, 2);
  surroundings.set_type(0, 0, Surroundings::FOOD);
  surroundings.set_type(1, 1, Surroundings::WALL);

  // Create copy
  Surroundings copy(surroundings);

  // Verify copy has same dimensions and values
  REQUIRE(copy.get_width() == 2);
  REQUIRE(copy.get_height() == 2);

  const auto& encoded = copy.get_encoded_surroundings();
  REQUIRE(encoded[0] == 1.0f);   // FOOD
  REQUIRE(encoded[3] == -1.0f);  // WALL
}
