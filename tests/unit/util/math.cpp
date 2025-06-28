#include "util/math.hpp"

#include <catch2/catch_test_macros.hpp>
#include <limits>

TEST_CASE("Math Utility Functions", "[math]") {
  SECTION("equal function with float") {
    SECTION("exact equality") {
      REQUIRE(Util::equal(1.0f, 1.0f));
      REQUIRE(Util::equal(0.0f, 0.0f));
      REQUIRE(Util::equal(-1.0f, -1.0f));
      REQUIRE(Util::equal(3.14159f, 3.14159f));
    }

    SECTION("near equality within epsilon") {
      float epsilon = std::numeric_limits<float>::epsilon();
      REQUIRE(Util::equal(1.0f, 1.0f + epsilon));
      REQUIRE(Util::equal(1.0f, 1.0f - epsilon));
      REQUIRE(Util::equal(0.0f, epsilon));
      REQUIRE(Util::equal(0.0f, -epsilon));
    }

    SECTION("inequality beyond epsilon") {
      float epsilon = std::numeric_limits<float>::epsilon();
      REQUIRE_FALSE(Util::equal(1.0f, 1.0f + 2.0f * epsilon));
      REQUIRE_FALSE(Util::equal(1.0f, 1.0f - 2.0f * epsilon));
      REQUIRE_FALSE(Util::equal(0.0f, 2.0f * epsilon));
      REQUIRE_FALSE(Util::equal(0.0f, -2.0f * epsilon));
    }

    SECTION("custom tolerance") {
      REQUIRE(Util::equal(1.0f, 1.1f, 0.2f));
      REQUIRE(Util::equal(1.0f, 0.9f, 0.2f));
      REQUIRE_FALSE(Util::equal(1.0f, 1.3f, 0.2f));
      REQUIRE_FALSE(Util::equal(1.0f, 0.7f, 0.2f));
    }

    SECTION("edge cases") {
      // Test with very small numbers
      REQUIRE(Util::equal(1e-10f, 1e-10f));
      REQUIRE_FALSE(Util::equal(1e-10f, 2e-10f));

      // Test with very large numbers
      REQUIRE(Util::equal(1e10f, 1e10f));
      REQUIRE_FALSE(Util::equal(1e10f, 1.1e10f));

      // Test with infinity
      REQUIRE(Util::equal(std::numeric_limits<float>::infinity(),
                          std::numeric_limits<float>::infinity()));
      REQUIRE_FALSE(Util::equal(std::numeric_limits<float>::infinity(),
                                -std::numeric_limits<float>::infinity()));

      // Test with NaN
      REQUIRE_FALSE(Util::equal(std::numeric_limits<float>::quiet_NaN(),
                                std::numeric_limits<float>::quiet_NaN()));
      REQUIRE_FALSE(Util::equal(1.0f, std::numeric_limits<float>::quiet_NaN()));
    }
  }

  SECTION("equal function with double") {
    SECTION("exact equality") {
      REQUIRE(Util::equal(1.0, 1.0));
      REQUIRE(Util::equal(0.0, 0.0));
      REQUIRE(Util::equal(-1.0, -1.0));
      REQUIRE(Util::equal(3.14159265359, 3.14159265359));
    }

    SECTION("near equality within epsilon") {
      double epsilon = std::numeric_limits<double>::epsilon();
      REQUIRE(Util::equal(1.0, 1.0 + epsilon));
      REQUIRE(Util::equal(1.0, 1.0 - epsilon));
      REQUIRE(Util::equal(0.0, epsilon));
      REQUIRE(Util::equal(0.0, -epsilon));
    }

    SECTION("inequality beyond epsilon") {
      double epsilon = std::numeric_limits<double>::epsilon();
      REQUIRE_FALSE(Util::equal(1.0, 1.0 + 2.0 * epsilon));
      REQUIRE_FALSE(Util::equal(1.0, 1.0 - 2.0 * epsilon));
      REQUIRE_FALSE(Util::equal(0.0, 2.0 * epsilon));
      REQUIRE_FALSE(Util::equal(0.0, -2.0 * epsilon));
    }

    SECTION("custom tolerance") {
      REQUIRE(Util::equal(1.0, 1.1, 0.2));
      REQUIRE(Util::equal(1.0, 0.9, 0.2));
      REQUIRE_FALSE(Util::equal(1.0, 1.3, 0.2));
      REQUIRE_FALSE(Util::equal(1.0, 0.7, 0.2));
    }

    SECTION("edge cases") {
      // Test with very small numbers
      REQUIRE(Util::equal(1e-20, 1e-20));
      REQUIRE_FALSE(Util::equal(1e-20, 2e-20));

      // Test with very large numbers
      REQUIRE(Util::equal(1e20, 1e20));
      REQUIRE_FALSE(Util::equal(1e20, 1.1e20));

      // Test with infinity
      REQUIRE(Util::equal(std::numeric_limits<double>::infinity(),
                          std::numeric_limits<double>::infinity()));
      REQUIRE_FALSE(Util::equal(std::numeric_limits<double>::infinity(),
                                -std::numeric_limits<double>::infinity()));

      // Test with NaN
      REQUIRE_FALSE(Util::equal(std::numeric_limits<double>::quiet_NaN(),
                                std::numeric_limits<double>::quiet_NaN()));
      REQUIRE_FALSE(Util::equal(1.0, std::numeric_limits<double>::quiet_NaN()));
    }
  }

  SECTION("equal function with long double") {
    SECTION("exact equality") {
      REQUIRE(Util::equal(1.0L, 1.0L));
      REQUIRE(Util::equal(0.0L, 0.0L));
      REQUIRE(Util::equal(-1.0L, -1.0L));
    }

    SECTION("near equality within epsilon") {
      long double epsilon = std::numeric_limits<long double>::epsilon();
      REQUIRE(Util::equal(1.0L, 1.0L + epsilon));
      REQUIRE(Util::equal(1.0L, 1.0L - epsilon));
      REQUIRE(Util::equal(0.0L, epsilon));
      REQUIRE(Util::equal(0.0L, -epsilon));
    }

    SECTION("custom tolerance") {
      REQUIRE(Util::equal(1.0L, 1.1L, 0.2L));
      REQUIRE(Util::equal(1.0L, 0.9L, 0.2L));
      REQUIRE_FALSE(Util::equal(1.0L, 1.3L, 0.2L));
      REQUIRE_FALSE(Util::equal(1.0L, 0.7L, 0.2L));
    }
  }

  SECTION("tolerance behavior") {
    SECTION("zero tolerance") {
      REQUIRE(Util::equal(1.0f, 1.0f, 0.0f));
      REQUIRE_FALSE(Util::equal(1.0f, 1.0f + std::numeric_limits<float>::epsilon(), 0.0f));
    }

    SECTION("large tolerance") {
      REQUIRE(Util::equal(1.0f, 10.0f, 10.0f));
      REQUIRE(Util::equal(1.0f, -5.0f, 10.0f));
      REQUIRE_FALSE(Util::equal(1.0f, 15.0f, 10.0f));
    }

    SECTION("negative tolerance") {
      // With negative tolerance, only exact equality should pass
      REQUIRE(Util::equal(1.0f, 1.0f, -1.0f));
      REQUIRE_FALSE(Util::equal(1.0f, 1.1f, -1.0f));
    }
  }
}