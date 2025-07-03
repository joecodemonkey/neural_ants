#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "game.hpp"

TEST_CASE("Game save and load functionality", "[game]") {
  const std::string test_save_file = "test_save.json";
  const std::string non_existent_file = "non_existent_file.json";

  // Clean up any existing test files
  if (std::filesystem::exists(test_save_file)) {
    std::filesystem::remove(test_save_file);
  }

  SECTION("Save game creates valid JSON file") {
    Game game;

    // Modify some game state to make the save more interesting
    auto& camera = game.get_camera();
    camera.offset = {100.0f, 200.0f};
    camera.target = {300.0f, 400.0f};
    camera.rotation = 45.0f;
    camera.zoom = 2.0f;

    game.set_target_fps(120);

    // Save the game
    auto save_result = game.save_game(test_save_file);
    REQUIRE(save_result.has_value());

    // Verify file was created
    REQUIRE(std::filesystem::exists(test_save_file));

    // Read and parse the saved file
    std::ifstream file(test_save_file);
    REQUIRE(file.is_open());

    nlohmann::json saved_data;
    file >> saved_data;

    // Verify required fields exist
    REQUIRE(saved_data.contains("version"));
    REQUIRE(saved_data.contains("timestamp"));
    REQUIRE(saved_data.contains("camera"));
    REQUIRE(saved_data.contains("fps"));
    REQUIRE(saved_data.contains("camera_speed"));
    REQUIRE(saved_data.contains("world"));

    // Verify version
    REQUIRE(saved_data["version"] == "1.0");

    // Verify timestamp is a positive number
    REQUIRE(saved_data["timestamp"].is_number());
    REQUIRE(saved_data["timestamp"].get<int64_t>() > 0);

    // Verify camera data
    auto saved_camera = saved_data["camera"];
    REQUIRE(saved_camera.contains("offset"));
    REQUIRE(saved_camera.contains("target"));
    REQUIRE(saved_camera.contains("rotation"));
    REQUIRE(saved_camera.contains("zoom"));

    REQUIRE(saved_camera["offset"]["x"] == 100.0f);
    REQUIRE(saved_camera["offset"]["y"] == 200.0f);
    REQUIRE(saved_camera["target"]["x"] == 300.0f);
    REQUIRE(saved_camera["target"]["y"] == 400.0f);
    REQUIRE(saved_camera["rotation"] == 45.0f);
    REQUIRE(saved_camera["zoom"] == 2.0f);

    // Verify fps
    REQUIRE(saved_data["fps"] == 120);

    // Verify world data exists
    REQUIRE(saved_data["world"].is_object());
  }

  SECTION("Load game restores state correctly") {
    Game original_game;

    // Set up original game state
    auto& camera = original_game.get_camera();
    camera.offset = {150.0f, 250.0f};
    camera.target = {350.0f, 450.0f};
    camera.rotation = 30.0f;
    camera.zoom = 1.5f;

    original_game.set_target_fps(90);

    // Save the original game
    auto save_result = original_game.save_game(test_save_file);
    REQUIRE(save_result.has_value());

    // Create a new game and load the saved state
    Game loaded_game;

    // Modify the new game to different values to ensure loading actually changes them
    auto& new_camera = loaded_game.get_camera();
    new_camera.offset = {999.0f, 999.0f};
    new_camera.target = {999.0f, 999.0f};
    new_camera.rotation = 999.0f;
    new_camera.zoom = 999.0f;

    loaded_game.set_target_fps(999);

    // Load the saved game
    auto load_result = loaded_game.load_game(test_save_file);
    REQUIRE(load_result.has_value());

    // Verify the loaded game has the same state as the original
    auto& loaded_camera = loaded_game.get_camera();
    REQUIRE(loaded_camera.offset.x == 150.0f);
    REQUIRE(loaded_camera.offset.y == 250.0f);
    REQUIRE(loaded_camera.target.x == 350.0f);
    REQUIRE(loaded_camera.target.y == 450.0f);
    REQUIRE(loaded_camera.rotation == 30.0f);
    REQUIRE(loaded_camera.zoom == 1.5f);

    REQUIRE(loaded_game.get_target_fps() == 90);
  }

  SECTION("Load game with non-existent file returns error") {
    Game game;

    auto load_result = game.load_game(non_existent_file);
    REQUIRE_FALSE(load_result.has_value());
    REQUIRE_THAT(load_result.error(),
                 Catch::Matchers::ContainsSubstring("Failed to open file for reading"));
  }

  SECTION("Load game with invalid JSON returns error") {
    // Create a file with invalid JSON
    std::ofstream invalid_file(test_save_file);
    invalid_file << "invalid json content";
    invalid_file.close();

    Game game;
    auto load_result = game.load_game(test_save_file);
    REQUIRE_FALSE(load_result.has_value());
    REQUIRE_THAT(load_result.error(),
                 Catch::Matchers::ContainsSubstring("Invalid save file format"));
  }

  SECTION("Load game with unsupported version returns error") {
    // Create a save file with unsupported version
    nlohmann::json invalid_save = {{"version", "2.0"},
                                   {"timestamp", 1234567890},
                                   {"camera",
                                    {{"offset", {{"x", 0.0f}, {"y", 0.0f}}},
                                     {"target", {{"x", 0.0f}, {"y", 0.0f}}},
                                     {"rotation", 0.0f},
                                     {"zoom", 1.0f}}},
                                   {"fps", 60},
                                   {"camera_speed", 0.0f},
                                   {"world", {}}};

    std::ofstream file(test_save_file);
    file << invalid_save.dump(2);
    file.close();

    Game game;
    auto load_result = game.load_game(test_save_file);
    REQUIRE_FALSE(load_result.has_value());
    REQUIRE_THAT(load_result.error(),
                 Catch::Matchers::ContainsSubstring("Unsupported save file version"));
  }

  SECTION("Load game with missing required fields returns error") {
    // Create a save file missing required fields
    nlohmann::json incomplete_save = {
        {"version", "1.0"}, {"timestamp", 1234567890}  // Missing camera, fps, camera_speed, world
    };

    std::ofstream file(test_save_file);
    file << incomplete_save.dump(2);
    file.close();

    Game game;
    auto load_result = game.load_game(test_save_file);
    REQUIRE_FALSE(load_result.has_value());
    REQUIRE_THAT(load_result.error(),
                 Catch::Matchers::ContainsSubstring("Invalid save file format"));
  }

  SECTION("Delete save removes file") {
    Game game;

    // Create a save file first
    auto save_result = game.save_game(test_save_file);
    REQUIRE(save_result.has_value());
    REQUIRE(std::filesystem::exists(test_save_file));

    // Delete the save file
    auto delete_result = game.delete_save(test_save_file);
    REQUIRE(delete_result.has_value());
    REQUIRE_FALSE(std::filesystem::exists(test_save_file));
  }

  SECTION("Delete save with non-existent file returns error") {
    Game game;

    auto delete_result = game.delete_save(non_existent_file);
    REQUIRE_FALSE(delete_result.has_value());
    REQUIRE_THAT(delete_result.error(), Catch::Matchers::ContainsSubstring("File does not exist"));
  }

  SECTION("Save and load preserves world state") {
    Game original_game;

    // Modify world state (this would require access to world methods)
    // For now, we'll just verify that the world data is included in the save
    auto save_result = original_game.save_game(test_save_file);
    REQUIRE(save_result.has_value());

    // Read the saved file
    std::ifstream file(test_save_file);
    nlohmann::json saved_data;
    file >> saved_data;

    // Verify world data is present and is an object
    REQUIRE(saved_data.contains("world"));
    REQUIRE(saved_data["world"].is_object());

    // Load into new game
    Game loaded_game;
    auto load_result = loaded_game.load_game(test_save_file);
    REQUIRE(load_result.has_value());

    // Verify both games have world objects (basic check)
    REQUIRE_NOTHROW(original_game.get_world());
    REQUIRE_NOTHROW(loaded_game.get_world());
  }

  SECTION("Multiple save and load operations work correctly") {
    Game game;

    // First save
    game.set_target_fps(60);
    auto save1_result = game.save_game(test_save_file);
    REQUIRE(save1_result.has_value());

    // Modify and save again
    game.set_target_fps(120);
    auto save2_result = game.save_game(test_save_file);
    REQUIRE(save2_result.has_value());

    // Load and verify the second save
    Game loaded_game;
    auto load_result = loaded_game.load_game(test_save_file);
    REQUIRE(load_result.has_value());
    REQUIRE(loaded_game.get_target_fps() == 120);
  }

  // Clean up test file
  if (std::filesystem::exists(test_save_file)) {
    std::filesystem::remove(test_save_file);
  }
}