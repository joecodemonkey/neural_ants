#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "containers/circular_stats.hpp"

using Catch::Approx;

TEST_CASE("CircularStats clear functionality", "[circular_stats]") {
  SECTION("Clear empty stats") {
    Containers::CircularStats<float> stats;

    stats.clear();

    REQUIRE(stats.get_seen() == 0);
    REQUIRE(stats.get_mean() == Approx(0.0f));
    REQUIRE(stats.get_max_data_points() == 100);
  }

  SECTION("Clear stats with data") {
    Containers::CircularStats<float> stats;

    stats.add_data(10.0f);
    stats.add_data(20.0f);
    stats.add_data(30.0f);

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(stats.get_mean() == Approx(20.0f));

    stats.clear();

    REQUIRE(stats.get_seen() == 0);
    REQUIRE(stats.get_mean() == Approx(0.0f));
    REQUIRE(stats.get_max_data_points() == 100);
  }

  SECTION("Clear stats with custom max data points") {
    Containers::CircularStats<float> stats;
    stats.set_max_data_points(5);

    stats.add_data(10.0f);
    stats.add_data(20.0f);

    REQUIRE(stats.get_seen() == 2);
    REQUIRE(stats.get_mean() == Approx(15.0f));
    REQUIRE(stats.get_max_data_points() == 5);

    stats.clear();

    REQUIRE(stats.get_seen() == 0);
    REQUIRE(stats.get_mean() == Approx(0.0f));
    REQUIRE(stats.get_max_data_points() == 5);
  }

  SECTION("Clear and add new data") {
    Containers::CircularStats<float> stats;

    stats.add_data(10.0f);
    stats.add_data(20.0f);
    stats.clear();

    // Add new data after clearing
    stats.add_data(5.0f);
    stats.add_data(15.0f);

    REQUIRE(stats.get_seen() == 2);
    REQUIRE(stats.get_mean() == Approx(10.0f));
  }

  SECTION("Clear with different data types") {
    Containers::CircularStats<int> stats;

    stats.add_data(1);
    stats.add_data(2);
    stats.add_data(3);

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(stats.get_mean() == 2);

    stats.clear();

    REQUIRE(stats.get_seen() == 0);
    REQUIRE(stats.get_mean() == 0);
  }

  SECTION("Multiple clear operations") {
    Containers::CircularStats<float> stats;

    stats.add_data(10.0f);
    stats.clear();
    stats.add_data(20.0f);
    stats.clear();
    stats.add_data(30.0f);

    REQUIRE(stats.get_seen() == 1);
    REQUIRE(stats.get_mean() == Approx(30.0f));
  }

  SECTION("Clear after circular behavior") {
    Containers::CircularStats<float> stats;
    stats.set_max_data_points(2);

    stats.add_data(10.0f);
    stats.add_data(20.0f);
    stats.add_data(30.0f);

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(stats.get_mean() == Approx(25.0f));

    stats.clear();

    REQUIRE(stats.get_seen() == 0);
    REQUIRE(stats.get_mean() == Approx(0.0f));
  }
}