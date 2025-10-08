#include <fmt/format.h>
#include <raylib.h>
#include <raymath.h>

#include <chrono>
#include <game.hpp>
#include <nlohmann/json.hpp>
#include <util/file.hpp>
#include <util/serialization.hpp>
#include <world.hpp>
#include <ant_renderer.hpp>

Game::Game() : _ui(*this), _input(*this), _updateSpeed(1LL) {
  _camera = {.offset = Vector2Zero(), .target = Vector2Zero(), .rotation = 0.0F, .zoom = 1.0f};
  _world.get_population().set_size(100);
  _world.get_resources().set_food_count(50);  // Reduced from 200 for stronger selection pressure
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
      _world.set_texture_cache(_textureCache);

      texturesLoaded = true;
    }

    BeginMode2D(_camera);

    if (!_ui.paused()) {
      _input.update(time);  // Move outside loop - only once per frame
      for (auto count = 0; count < _updateSpeed; ++count) {
        _world.update(time);  // Only world simulation repeats
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
    _antRenderer.draw(_world.get_population(), *_textureCache);
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
    return;  // Ignore the request to increase speed
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

  // Load UI sprites dynamically
  if (!_textureCache->load_textures("assets/sprites/ui")) {
    throw std::runtime_error("Failed to load UI textures");
  }

  // Set default texture (ui_close must exist)
  if (!_textureCache->has_texture("ui_close")) {
    throw std::runtime_error("Failed to load required texture: 'ui_close'");
  }
  _textureCache->set_default("ui_close");

  // Load ant sprites dynamically
  if (!_textureCache->load_textures("assets/sprites/ants")) {
    throw std::runtime_error("Failed to load ant textures");
  }

  // Load food sprites dynamically
  if (!_textureCache->load_textures("assets/sprites/food")) {
    throw std::runtime_error("Failed to load food textures");
  }
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
    auto content = Util::File::read_file(filename);
    if (!content) {
      return std::unexpected(content.error());
    }

    nlohmann::json save_data = nlohmann::json::parse(*content);

    if (save_data.empty()) {
      return std::unexpected("Save file is empty or invalid JSON");
    }

    auto version = save_data.at("version").get<std::string>();
    if (version != "1.0") {
      return std::unexpected(fmt::format("Unsupported save file version: {}", version));
    }

    _camera = Util::camera2d_from_json(save_data.at("camera"));
    _fps = save_data.at("fps").get<int>();
    _cameraSpeed = save_data.at("camera_speed").get<float>();
    _world = World(save_data.at("world"));
    _world.set_texture_cache(_textureCache);

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

auto Game::get_texture_cache() -> std::shared_ptr<TextureCache> {
  return _textureCache;
}
