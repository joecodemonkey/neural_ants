#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include "population.hpp"
#include "world.hpp"

TEST_CASE("Population JSON serialization", "[population]") {
  World world;

  SECTION("Basic to_json functionality") {
    Population population(world);
    population.set_size(3);

    nlohmann::json j = population.to_json();

    REQUIRE(j.contains("size"));
    REQUIRE(j["size"] == 3);
    REQUIRE(j.contains("ants"));
    REQUIRE(j["ants"].is_array());
    REQUIRE(j.contains("pangenome"));
    REQUIRE(j["pangenome"].is_array());
  }

  SECTION("JSON round trip with size") {
    Population original(world);
    original.set_size(7);

    nlohmann::json j = original.to_json();
    Population restored(j, world);

    REQUIRE(restored == original);
  }

  SECTION("JSON round trip with texture path") {
    Population original(world);
    original.set_size(5);

    nlohmann::json j = original.to_json();
    Population restored(j, world);

    REQUIRE(restored.get_size() == original.get_size());
  }

  SECTION("Empty population serialization") {
    Population population(world);
    population.set_size(0);

    nlohmann::json j = population.to_json();

    REQUIRE(j["size"] == 0);
    REQUIRE(j["ants"].is_array());
    REQUIRE(j["ants"].empty());
    REQUIRE(j["pangenome"].is_array());
  }

  SECTION("JSON constructor with valid data") {
    nlohmann::json j;
    j["size"] = 5;
    j["ants"] = nlohmann::json::array();
    j["pangenome"] = nlohmann::json::array();

    REQUIRE_NOTHROW(Population(j, world));

    Population population(j, world);
    REQUIRE(population.get_size() == 5);
  }

  SECTION("JSON constructor with missing required fields") {
    nlohmann::json j;
    j["size"] = 5;
    // Missing ants and pangenome arrays

    REQUIRE_THROWS(Population(j, world));
  }

  SECTION("JSON constructor with invalid size") {
    nlohmann::json j;
    j["size"] = "invalid";  // Should be int
    j["ants"] = nlohmann::json::array();
    j["pangenome"] = nlohmann::json::array();

    REQUIRE_THROWS(Population(j, world));
  }
}