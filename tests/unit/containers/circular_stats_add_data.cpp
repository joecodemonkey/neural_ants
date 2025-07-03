#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "containers/circular_stats.hpp"

using Catch::Approx;

TEST_CASE("CircularStats add_data functionality", "[circular_stats]") {
  SECTION("Adding single data point with float") {
    Containers::CircularStats<float> stats;

    stats.add_data(10.0f);

    REQUIRE(stats.get_seen() == 1);
    REQUIRE(stats.get_mean() == Approx(10.0f));
  }

  SECTION("Adding multiple data points with float") {
    Containers::CircularStats<float> stats;

    stats.add_data(10.0f);
    stats.add_data(20.0f);
    stats.add_data(30.0f);

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(stats.get_mean() == Approx(20.0f));
  }

  SECTION("Adding data with double") {
    Containers::CircularStats<double> stats;

    stats.add_data(5.5);
    stats.add_data(10.5);

    REQUIRE(stats.get_seen() == 2);
    REQUIRE(stats.get_mean() == Approx(8.0));
  }

  SECTION("Adding data with int") {
    Containers::CircularStats<int> stats;

    stats.add_data(5);
    stats.add_data(15);
    stats.add_data(25);

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(stats.get_mean() == 15);

    SECTION("Adding zero values") {
      Containers::CircularStats<float> stats;

      stats.add_data(0.0f);
      stats.add_data(0.0f);

      REQUIRE(stats.get_seen() == 2);
      REQUIRE(stats.get_mean() == Approx(0.0f));
    }

    SECTION("Adding negative values") {
      Containers::CircularStats<float> stats;

      stats.add_data(-10.0f);
      stats.add_data(10.0f);

      REQUIRE(stats.get_seen() == 2);
      REQUIRE(stats.get_mean() == Approx(0.0f));
    }

    SECTION("Adding many data points") {
      Containers::CircularStats<float> stats;

      for (int i = 1; i <= 10; ++i) {
        stats.add_data(static_cast<float>(i));
      }

      REQUIRE(stats.get_seen() == 10);
      REQUIRE(stats.get_mean() == Approx(5.5f));
    }
  }
}