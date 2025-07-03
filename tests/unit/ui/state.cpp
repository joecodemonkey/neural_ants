#include "ui/state.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("UI State Component Management", "[ui][state]") {
  UI::State state;

  SECTION("Initial state is empty") {
    REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));
    REQUIRE_FALSE(state.is_maximized(UI::State::MEAN_FITNESS));
    REQUIRE_FALSE(state.is_maximized(UI::State::SAVELOAD));
  }

  SECTION("maximize function") {
    SECTION("maximize single component") {
      state.maximize(UI::State::SETTINGS);
      REQUIRE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE_FALSE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE_FALSE(state.is_maximized(UI::State::SAVELOAD));
    }

    SECTION("maximize multiple components") {
      state.maximize(UI::State::SETTINGS);
      state.maximize(UI::State::MEAN_FITNESS);

      REQUIRE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE_FALSE(state.is_maximized(UI::State::SAVELOAD));
    }

    SECTION("maximize same component multiple times") {
      state.maximize(UI::State::SETTINGS);
      state.maximize(UI::State::SETTINGS);
      state.maximize(UI::State::SETTINGS);

      REQUIRE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE_FALSE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE_FALSE(state.is_maximized(UI::State::SAVELOAD));
    }

    SECTION("maximize all components") {
      state.maximize(UI::State::SETTINGS);
      state.maximize(UI::State::MEAN_FITNESS);
      state.maximize(UI::State::SAVELOAD);

      REQUIRE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE(state.is_maximized(UI::State::SAVELOAD));
    }
  }

  SECTION("minimize function") {
    SECTION("minimize non-maximized component") {
      state.minimize(UI::State::SETTINGS);
      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));
    }

    SECTION("minimize maximized component") {
      state.maximize(UI::State::SETTINGS);
      REQUIRE(state.is_maximized(UI::State::SETTINGS));

      state.minimize(UI::State::SETTINGS);
      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));
    }

    SECTION("minimize component multiple times") {
      state.maximize(UI::State::SETTINGS);
      state.minimize(UI::State::SETTINGS);
      state.minimize(UI::State::SETTINGS);
      state.minimize(UI::State::SETTINGS);

      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));
    }

    SECTION("minimize one component doesn't affect others") {
      state.maximize(UI::State::SETTINGS);
      state.maximize(UI::State::MEAN_FITNESS);
      state.maximize(UI::State::SAVELOAD);

      state.minimize(UI::State::SETTINGS);

      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE(state.is_maximized(UI::State::SAVELOAD));
    }
  }

  SECTION("is_maximized function") {
    SECTION("returns false for non-maximized components") {
      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE_FALSE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE_FALSE(state.is_maximized(UI::State::SAVELOAD));
    }

    SECTION("returns true for maximized components") {
      state.maximize(UI::State::SETTINGS);
      state.maximize(UI::State::MEAN_FITNESS);

      REQUIRE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE_FALSE(state.is_maximized(UI::State::SAVELOAD));
    }

    SECTION("returns correct state after minimize") {
      state.maximize(UI::State::SETTINGS);
      REQUIRE(state.is_maximized(UI::State::SETTINGS));

      state.minimize(UI::State::SETTINGS);
      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));
    }
  }

  SECTION("toggle function") {
    SECTION("toggle non-maximized component maximizes it") {
      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));

      state.toggle(UI::State::SETTINGS);
      REQUIRE(state.is_maximized(UI::State::SETTINGS));
    }

    SECTION("toggle maximized component minimizes it") {
      state.maximize(UI::State::SETTINGS);
      REQUIRE(state.is_maximized(UI::State::SETTINGS));

      state.toggle(UI::State::SETTINGS);
      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));
    }

    SECTION("toggle multiple times") {
      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));

      state.toggle(UI::State::SETTINGS);
      REQUIRE(state.is_maximized(UI::State::SETTINGS));

      state.toggle(UI::State::SETTINGS);
      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));

      state.toggle(UI::State::SETTINGS);
      REQUIRE(state.is_maximized(UI::State::SETTINGS));
    }

    SECTION("toggle different components independently") {
      state.toggle(UI::State::SETTINGS);
      state.toggle(UI::State::MEAN_FITNESS);

      REQUIRE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE_FALSE(state.is_maximized(UI::State::SAVELOAD));

      state.toggle(UI::State::SETTINGS);

      REQUIRE_FALSE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE_FALSE(state.is_maximized(UI::State::SAVELOAD));
    }
  }

  SECTION("Component enum values") {
    SECTION("all component values are distinct") {
      REQUIRE(static_cast<int>(UI::State::SETTINGS) != static_cast<int>(UI::State::MEAN_FITNESS));
      REQUIRE(static_cast<int>(UI::State::SETTINGS) != static_cast<int>(UI::State::SAVELOAD));
      REQUIRE(static_cast<int>(UI::State::MEAN_FITNESS) != static_cast<int>(UI::State::SAVELOAD));
    }

    SECTION("components can be used as set keys") {
      state.maximize(UI::State::SETTINGS);
      state.maximize(UI::State::MEAN_FITNESS);
      state.maximize(UI::State::SAVELOAD);

      REQUIRE(state.is_maximized(UI::State::SETTINGS));
      REQUIRE(state.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE(state.is_maximized(UI::State::SAVELOAD));
    }
  }

  SECTION("State object independence") {
    UI::State state1;
    UI::State state2;

    SECTION("states are independent") {
      state1.maximize(UI::State::SETTINGS);
      state2.maximize(UI::State::MEAN_FITNESS);

      REQUIRE(state1.is_maximized(UI::State::SETTINGS));
      REQUIRE_FALSE(state1.is_maximized(UI::State::MEAN_FITNESS));

      REQUIRE_FALSE(state2.is_maximized(UI::State::SETTINGS));
      REQUIRE(state2.is_maximized(UI::State::MEAN_FITNESS));
    }

    SECTION("operations on one state don't affect the other") {
      state1.maximize(UI::State::SETTINGS);
      state1.maximize(UI::State::MEAN_FITNESS);

      state2.maximize(UI::State::SAVELOAD);

      REQUIRE(state1.is_maximized(UI::State::SETTINGS));
      REQUIRE(state1.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE_FALSE(state1.is_maximized(UI::State::SAVELOAD));

      REQUIRE_FALSE(state2.is_maximized(UI::State::SETTINGS));
      REQUIRE_FALSE(state2.is_maximized(UI::State::MEAN_FITNESS));
      REQUIRE(state2.is_maximized(UI::State::SAVELOAD));
    }
  }
}