#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "containers/circular_stats.hpp"

using Catch::Approx;

// Custom type that satisfies the Summable concept
struct CustomSummable {
  double value;

  CustomSummable() : value(0.0) {}
  CustomSummable(double v) : value(v) {}

  CustomSummable operator+(const CustomSummable& other) const {
    return CustomSummable(value + other.value);
  }

  CustomSummable& operator+=(const CustomSummable& other) {
    value += other.value;
    return *this;
  }

  CustomSummable operator/(std::size_t divisor) const {
    return CustomSummable(value / static_cast<double>(divisor));
  }

  operator double() const {
    return value;
  }

  bool operator==(const CustomSummable& other) const {
    return value == other.value;
  }
};

TEST_CASE("CircularStats with custom summable types", "[circular_stats]") {
  SECTION("CustomSummable type basic operations") {
    Containers::CircularStats<CustomSummable> stats;

    stats.add_data(CustomSummable(10.0));
    stats.add_data(CustomSummable(20.0));
    stats.add_data(CustomSummable(30.0));

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(static_cast<double>(stats.get_mean()) == Approx(20.0));
  }

  SECTION("CustomSummable type circular behavior") {
    Containers::CircularStats<CustomSummable> stats;
    stats.set_max_data_points(2);

    stats.add_data(CustomSummable(10.0));
    stats.add_data(CustomSummable(20.0));
    stats.add_data(CustomSummable(30.0));

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(static_cast<double>(stats.get_mean()) == Approx(25.0));
  }

  SECTION("CustomSummable type clear functionality") {
    Containers::CircularStats<CustomSummable> stats;

    stats.add_data(CustomSummable(10.0));
    stats.add_data(CustomSummable(20.0));
    stats.clear();

    REQUIRE(stats.get_seen() == 0);
    REQUIRE(static_cast<double>(stats.get_mean()) == Approx(0.0));
  }

  SECTION("CustomSummable type with negative values") {
    Containers::CircularStats<CustomSummable> stats;

    stats.add_data(CustomSummable(-10.0));
    stats.add_data(CustomSummable(10.0));

    REQUIRE(stats.get_seen() == 2);
    REQUIRE(static_cast<double>(stats.get_mean()) == Approx(0.0));
  }

  SECTION("CustomSummable type max data points") {
    Containers::CircularStats<CustomSummable> stats;
    stats.set_max_data_points(3);

    for (int i = 1; i <= 5; ++i) {
      stats.add_data(CustomSummable(static_cast<double>(i)));
    }

    REQUIRE(stats.get_seen() == 5);
    REQUIRE(static_cast<double>(stats.get_mean()) == Approx(4.0));  // (3 + 4 + 5) / 3
  }

  SECTION("CustomSummable type with zero values") {
    Containers::CircularStats<CustomSummable> stats;

    stats.add_data(CustomSummable(0.0));
    stats.add_data(CustomSummable(0.0));
    stats.add_data(CustomSummable(0.0));

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(static_cast<double>(stats.get_mean()) == Approx(0.0));
  }

  SECTION("CustomSummable type edge case - single value") {
    Containers::CircularStats<CustomSummable> stats;
    stats.set_max_data_points(1);

    stats.add_data(CustomSummable(10.0));
    stats.add_data(CustomSummable(20.0));
    stats.add_data(CustomSummable(30.0));

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(static_cast<double>(stats.get_mean()) == Approx(30.0));
  }
}