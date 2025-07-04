#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "containers/circular_stats.hpp"

using Catch::Approx;

TEST_CASE("CircularStats max data points functionality", "[circular_stats]") {
  SECTION("Default max data points") {
    Containers::CircularStats<float> stats;

    REQUIRE(stats.get_max_data_points() == 100);
  }

  SECTION("Setting max data points") {
    Containers::CircularStats<float> stats;

    stats.set_max_data_points(5);
    REQUIRE(stats.get_max_data_points() == 5);
  }

  SECTION("Circular behavior - exceeding max data points") {
    Containers::CircularStats<float> stats;
    stats.set_max_data_points(3);

    // Add 5 data points, but only the last 3 should be kept
    stats.add_data(10.0f);  // seen: 1, mean: 10.0
    stats.add_data(20.0f);  // seen: 2, mean: 15.0
    stats.add_data(30.0f);  // seen: 3, mean: 20.0
    stats.add_data(40.0f);  // seen: 4, mean: 30.0 (10 removed)
    stats.add_data(50.0f);  // seen: 5, mean: 40.0 (20 removed)

    REQUIRE(stats.get_seen() == 5);
    REQUIRE(stats.get_mean() == Approx(40.0f));  // (30 + 40 + 50) / 3
  }

  SECTION("Circular behavior with different data types") {
    Containers::CircularStats<int> stats;
    stats.set_max_data_points(2);

    stats.add_data(1);
    stats.add_data(2);
    stats.add_data(3);
    stats.add_data(4);

    REQUIRE(stats.get_seen() == 4);
    REQUIRE(stats.get_mean() == 3);  // (3 + 4) / 2
  }

  SECTION("Large max data points") {
    Containers::CircularStats<float> stats;
    stats.set_max_data_points(1000);

    // Add data up to the limit
    for (int i = 1; i <= 1000; ++i) {
      stats.add_data(static_cast<float>(i));
    }

    REQUIRE(stats.get_seen() == 1000);
    REQUIRE(stats.get_mean() == Approx(500.5f));  // Average of 1 to 1000

    // Add one more to test circular behavior
    stats.add_data(2000.0f);

    REQUIRE(stats.get_seen() == 1001);
    REQUIRE(stats.get_mean() == Approx(502.5f));  // Average of 2 to 1000 plus 2000
  }

  SECTION("Changing max data points after adding data") {
    Containers::CircularStats<float> stats;

    // Add some data
    stats.add_data(10.0f);
    stats.add_data(20.0f);
    stats.add_data(30.0f);

    REQUIRE(stats.get_seen() == 3);
    REQUIRE(stats.get_mean() == Approx(20.0f));

    // Reduce max data points
    stats.set_max_data_points(2);

    REQUIRE(stats.get_max_data_points() == 2);
    REQUIRE(stats.get_seen() == 3);
    REQUIRE(stats.get_mean() == Approx(25.0f));
  }
}