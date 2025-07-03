#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "containers/circular_stats.hpp"

using Catch::Approx;

TEST_CASE("CircularStats initialization", "[circular_stats]") {
  SECTION("Default constructor with float") {
    Containers::CircularStats<float> stats;

    REQUIRE(stats.get_seen() == 0);
    REQUIRE(stats.get_mean() == Approx(0.0f));
    REQUIRE(stats.get_max_data_points() == 100);
  }

  SECTION("Default constructor with double") {
    Containers::CircularStats<double> stats;

    REQUIRE(stats.get_seen() == 0);
    REQUIRE(stats.get_mean() == Approx(0.0));
    REQUIRE(stats.get_max_data_points() == 100);
  }

  SECTION("Default constructor with int") {
    Containers::CircularStats<int> stats;

    REQUIRE(stats.get_seen() == 0);
    REQUIRE(stats.get_mean() == 0);
    REQUIRE(stats.get_max_data_points() == 100);
  }
}