#include "game.hpp"

#include <fmt/format.h>

#include <chrono>
#include <nlohmann/json.hpp>

#include "raylib.h"
#include "raymath.h"
#include "util/file.hpp"
#include "util/serialization.hpp"
#include "world.hpp"

Game::Game() : _ui(*this), _input(*this), _updateSpeed(1LL) {
  _camera = {.offset = Vector2Zero(), .target = Vector2Zero(), .rotation = 0.0F, .zoom = 1.0f};
  _world.get_population().set_size(50);
  _world.get_population().set_texture_path("./ant.png");
  _world.get_resources().set_food_count(100);
}

auto Game::run() -> void {
  if (!_raylibInitialized) {
    initialize_raylib();
  }

  bool texturesLoaded = false;

  while (!WindowShouldClose()) {
    float time = GetFrameTime();
    BeginDrawing();
    if (!texturesLoaded) {
      load_textures();
      _ui.add_texture_cache(_textureCache);
      texturesLoaded = true;
    }

    BeginMode2D(_camera);

    if (!_ui.paused()) {
      for (auto count = 0; count < _updateSpeed; ++count) {
        _input.update(time);
        _world.update(time);
      }
    }
    
    // Monitor FPS and reduce speed if it drops below 10 FPS
    // Wait 2 seconds before making safety adjustments to prevent rapid drops
    float currentTime = GetTime();
    
    int currentFPS = GetFPS();
    if (currentFPS < 10 && _updateSpeed > 1LL && (currentTime - _lastSpeedAdjustmentTime) >= 2.0f) {
      _updateSpeed = _updateSpeed / 2LL;
      // Ensure speed never goes below 1x
      if (_updateSpeed < 1LL) {
        _updateSpeed = 1LL;
      }
      _lastSpeedAdjustmentTime = currentTime;
    }

    ClearBackground(BLACK);
    _world.draw();
    EndMode2D();
    _ui.draw(time);

    EndDrawing();
  }

  CloseWindow();
}

auto Game::get_camera() const -> const Camera2D& {
  return _camera;
}

auto Game::get_camera() -> Camera2D& {
  return _camera;
}

auto Game::set_camera(const Camera2D& camera) -> void {
  _camera = camera;
}

auto Game::get_target_fps() const -> int {
  return _fps;
}

auto Game::set_target_fps(int fps) -> void {
  _fps = fps;
  if (_raylibInitialized) {
    SetTargetFPS(fps);
  }
}

auto Game::get_update_speed() const -> long long {
  return _updateSpeed;
}

auto Game::set_update_speed(long long speed) -> void {
  // Don't allow speed increases during FPS cooldown period
  float currentTime = GetTime();
  if (speed > _updateSpeed && (currentTime - _lastSpeedAdjustmentTime) < 2.0f) {
    return; // Ignore the request to increase speed
  }
  _updateSpeed = speed;
}

auto Game::initialize_raylib() -> void {
  if (!_raylibInitialized) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(_world.get_bounds().width, _world.get_bounds().height, "Neural Ants");
    SetTargetFPS(_fps);
    _raylibInitialized = true;
  }
}

auto Game::load_textures() -> void {
  _textureCache = std::make_shared<TextureCache>();
  if (!_textureCache->add_texture("close", "buttonX.png")) {
    throw std::runtime_error("Failed to load default texture: 'buttonX.png'");
  }
  _textureCache->set_default("close");
  _textureCache->add_texture("settings", "gear.png");
  _textureCache->add_texture("save", "save.png");
  _textureCache->add_texture("delete", "trashcan.png");
  _textureCache->add_texture("load", "import.png");
  _textureCache->add_texture("progress", "signal3.png");
  _textureCache->add_texture("exit", "exitRight.png");
  _textureCache->add_texture("fastForward", "fastForward.png");
  _textureCache->add_texture("rewind", "rewind.png");
  _textureCache->add_texture("ant", "default");
}

auto Game::save_game(const std::string& filename) const -> std::expected<void, std::string> {
  try {
    nlohmann::json save_data;

    // Add metadata
    save_data["version"] = "1.0";
    save_data["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
                                 std::chrono::system_clock::now().time_since_epoch())
                                 .count();

    // Add camera state using serialization utility
    save_data["camera"] = Util::camera2d_to_json(_camera);

    // Add game settings
    save_data["fps"] = _fps;
    save_data["camera_speed"] = _cameraSpeed;

    // Add world state
    save_data["world"] = _world.to_json();

    // Write to file
    return Util::File::write_file(filename, save_data.dump(2));

  } catch (const std::exception& e) {
    return std::unexpected(fmt::format("Failed to save game: {}", e.what()));
  }
}

auto Game::load_game(const std::string& filename) -> std::expected<void, std::string> {
  try {
    // Read file
    auto content = Util::File::read_file(filename);
    if (!content) {
      return std::unexpected(content.error());
    }

    // Parse JSON
    nlohmann::json save_data = nlohmann::json::parse(*content);

    // Validate version
    auto version = save_data.at("version").get<std::string>();
    if (version != "1.0") {
      return std::unexpected(fmt::format("Unsupported save file version: {}", version));
    }

    // Restore camera state using serialization utility
    _camera = Util::camera2d_from_json(save_data.at("camera"));

    // Restore game settings
    _fps = save_data.at("fps").get<int>();
    _cameraSpeed = save_data.at("camera_speed").get<float>();

    // Restore world state
    _world = World(save_data.at("world"));

    // Force texture loading and application to restored ants
    _world.get_population().update(0.0f);

    return {};

  } catch (const nlohmann::json::exception& e) {
    return std::unexpected(fmt::format("Invalid save file format: {}", e.what()));
  } catch (const std::exception& e) {
    return std::unexpected(fmt::format("Failed to load game: {}", e.what()));
  }
}

auto Game::delete_save(const std::string& filename) -> std::expected<void, std::string> {
  return Util::File::delete_file(filename);
}

auto Game::get_world() const -> const World& {
  return _world;
}

auto Game::get_world() -> World& {
  return _world;
}