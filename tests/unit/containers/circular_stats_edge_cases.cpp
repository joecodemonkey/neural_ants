#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "containers/circular_stats.hpp"

using Catch::Approx;

TEST_CASE("CircularStats edge cases", "[circular_stats]") {
  SECTION("Very large numbers") {
    Containers::CircularStats<double> stats;

    stats.add_data(1e15);
    stats.add_data(2e15);

    REQUIRE(stats.get_seen() == 2);
    REQUIRE(stats.get_mean() == Approx(1.5e15));
  }

  SECTION("Very small numbers") {
    Containers::CircularStats<double> stats;

    stats.add_data(1e-15);
    stats.add_data(2e-15);

    REQUIRE(stats.get_seen() == 2);
    REQUIRE(stats.get_mean() == Approx(1.5e-15));
  }

  SECTION("Mixed positive and negative large numbers") {
    Containers::CircularStats<double> stats;

    stats.add_data(1e10);
    stats.add_data(-1e10);

    REQUIRE(stats.get_seen() == 2);
    REQUIRE(stats.get_mean() == Approx(0.0));
  }

  SECTION("Max data points set to very large value") {
    Containers::CircularStats<float> stats;
    stats.set_max_data_points(1000000);

    REQUIRE(stats.get_max_data_points() == 1000000);

    // Add a reasonable amount of data
    for (int i = 1; i <= 100; ++i) {
      stats.add_data(static_cast<float>(i));
    }

    REQUIRE(stats.get_seen() == 100);
    REQUIRE(stats.get_mean() == Approx(50.5f));
  }

  SECTION("Zero max data points edge case") {
    Containers::CircularStats<float> stats;
    stats.set_max_data_points(0);

    stats.add_data(10.0f);
    stats.add_data(20.0f);
    stats.add_data(30.0f);

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(stats.get_mean() == Approx(0.0f));
  }

  SECTION("Single max data point") {
    Containers::CircularStats<float> stats;
    stats.set_max_data_points(1);

    for (int i = 1; i <= 10; ++i) {
      stats.add_data(static_cast<float>(i));
    }

    REQUIRE(stats.get_seen() == 10);
    REQUIRE(stats.get_mean() == Approx(10.0f));
  }

  SECTION("Precision test with floating point") {
    Containers::CircularStats<double> stats;

    stats.add_data(0.1);
    stats.add_data(0.2);
    stats.add_data(0.3);

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(stats.get_mean() == Approx(0.2));
  }

  SECTION("Integer division precision") {
    Containers::CircularStats<int> stats;

    stats.add_data(1);
    stats.add_data(2);
    stats.add_data(3);
    stats.add_data(4);

    REQUIRE(stats.get_seen() == 4);
    REQUIRE(stats.get_mean() == 2);
  }

  SECTION("Clear and immediately add data") {
    Containers::CircularStats<float> stats;

    stats.add_data(10.0f);
    stats.clear();
    stats.add_data(20.0f);

    REQUIRE(stats.get_seen() == 1);
    REQUIRE(stats.get_mean() == Approx(20.0f));
  }
}